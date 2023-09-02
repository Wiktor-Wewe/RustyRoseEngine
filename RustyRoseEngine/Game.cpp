#include "Game.h"

Game::Game()
{
    this->_gameStatus = false; 

    // load init
    if (!this->_loadInit()) {
        this->_setDefaultInit();
    }

    // load jumps
    if (!this->_loadJumps()) {
        return;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: initializing SDL: %s\n", SDL_GetError());
    }

    // initialization of SDL2 Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Error: cant initialize SDL: %s\n", SDL_GetError());
        return;
    }

    // initialization of TTF
    if (TTF_Init() == -1) {
        // Obs³uga b³êdu
        printf("Error: cant initialize SDL_ttf: %s\n", TTF_GetError());
        return;
    }
    
    // make window
    this->_window = SDL_CreateWindow("School Days: Rusty Rose Edition",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720, 0);

    // make renderer
    this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
    if (this->_renderer == NULL) {
        printf("Error: cant create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(this->_window);
        SDL_Quit();
        return;
    }

    // create mutex
    this->_eventMutex = SDL_CreateMutex();

    // set speed level
    this->_speedLevels = { 1, 2, 4, 16, 32 };
    this->_currentSpeedLevelIndex = 0;

    // make new content, scene, video decoder, timer and control
    this->_gameContext = new GameContext(this->_renderer);
    this->_scene = new Scene(this->_renderer);
    this->_vDecoder = new VDecoder(this->_renderer);
    this->_soloud = new SoLoud::Soloud();
    this->_soloud->init();
    this->_timer = new Timer();
    this->_control = Control();

    // set and load basic of system | set font from system files
    this->_gameContext->getSystem()->setSystem(this->_init.debugString + this->_init.linkToSys);
    this->_gameContext->getSystem()->loadSystem();
    this->_scene->setFont(this->_gameContext->getSystem()->getFont());
    this->_loadClickSe();

    this->_gameStatus = true;
}

void Game::play() // <- main play, automatic playing scripts
{
    // add starting script to context
    this->_gameContext->addScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_init.startScript + ".rose");
    this->_scriptHistory.push_back(this->_gameContext->getScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_init.startScript + ".rose"));
    bool play = true;

    while (play) {

        // if scripts is not empty, make load content for play
        if (!this->_gameContext->loadContentFromScripts()) {
            printf("Error - there is no content from script\n");
            return;
        }

        // get index of next script or end loop
        int nextScriptId = this->_playScripts();

        if (nextScriptId == -1) {
            play = false;
        }

        if (nextScriptId == Command::previousScript) {
            if (this->_scriptHistory.size() > 1) {
                this->_removeFrom(this->_gameContext->getLastScript(), this->_scriptHistory);
            }
        }

        // clear context to old script and free system images and sounds
        this->_gameContext->clear();
        this->_gameContext->getSystem()->freeSystem();

        // add next script
        if (nextScriptId >= 0) {
            this->_gameContext->addScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_findNameOfScriptById(nextScriptId) + ".rose");
            this->_scriptHistory.push_back(this->_gameContext->getScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_findNameOfScriptById(nextScriptId) + ".rose"));
        }
        else if (nextScriptId == Command::previousScript) {
            Script* previousScript = this->_scriptHistory[this->_scriptHistory.size() - 1];
            this->_gameContext->addScript(previousScript->getPath());
        }
        
        // clear scene
        this->_scene->clear(Scene::Clear::allExceptTextAndIndex);
        this->_scene->clear(Scene::Clear::text);
        this->_scene->clear(Scene::Clear::pathOptions);
    }
}

SDL_Renderer* Game::getRenderer()
{
    return this->_renderer;
}

SDL_Window* Game::getWindow()
{
    return this->_window;
}

bool Game::isGameGood()
{
    return this->_gameStatus;
}

int Game::_playScripts()
{
    bool pause = false;
    bool isOkayToSkip = true;
    Script::Event* setSELECT = nullptr;
    int extraCommand = -1;

    // load all events from scripts | but probably its only one in vector
    std::vector<Script::Event*> todo;
    std::vector<Script::Event*> ready;
    std::vector<Script::Event*> inprogres;

    Script::Time extraTimeToEndVoice; // <- extra time to make sure that all voice ends play
    extraTimeToEndVoice.millisecond = 500;

    for (int i = 0; i < this->_gameContext->getScripts().size(); i++) {
        auto _script = this->_gameContext->getScripts()[i];
        for (int j = 0; j < _script->getEvents().size(); j++) {
            auto _event = _script->getEvents()[j];
            if (_event->action == 0xCC05) {
                *_event->end = *_event->end + extraTimeToEndVoice;
            }
            if (_event->action == 0xCC0B) {
                isOkayToSkip = false;
                setSELECT = _event;
            }
            todo.push_back(_event);
        }
    }

    // reset timer and control
    this->_timer->reset();
    this->_control.clear();

    // make time to be sure that all current events ends load before use
    Script::Time timeForPrepare;
    timeForPrepare.second = 2; // <- add to init file?

    // make time to be sure that all event was free alter end | eg. not free voice that is currently playing
    Script::Time extraTime;
    extraTime.millisecond = 20;

    bool quit = false;
    bool firstPrepare = true;
    SDL_Event sdl_event;

    // fps
    const int TARGET_FPS = 24; // <- no need more | video are in 24 fps
    Uint32 frameStartTime;
    Uint32 frameEndTime;
    Uint32 frameTime;

    while (!quit)
    {
        frameStartTime = SDL_GetTicks();
        while (SDL_PollEvent(&sdl_event)){
            if (sdl_event.type == SDL_QUIT){ // <- close all app
                return -1;
            }
            // look for type sdl events and add this to this->control 
            else if (sdl_event.type == SDL_KEYDOWN) {
                switch (sdl_event.key.keysym.sym)
                {
                case SDLK_q:
                    this->_control.add(Control::quit);
                    break;

                case SDLK_n:
                    this->_control.add(Control::next);
                    break;

                case SDLK_SPACE:
                    this->_control.add(Control::pause);
                    break;

                case SDLK_LEFT:
                    this->_control.add(Control::left);
                    break;

                case SDLK_RIGHT:
                    this->_control.add(Control::right);
                    break;

                case SDLK_UP:
                    this->_control.add(Control::up);
                    break;

                case SDLK_DOWN:
                    this->_control.add(Control::down);
                    break;

                case SDLK_KP_ENTER:
                    this->_control.add(Control::enter);
                    break;

                case SDLK_BACKSPACE:
                    this->_control.add(Control::back);
                    break;

                case SDLK_s:
                    this->_control.add(Control::save);
                    break;

                case SDLK_l:
                    this->_control.add(Control::load);
                    break;
                }
            }
        }

        SDL_LockMutex(this->_eventMutex);
        
        // debug info
        this->_scene->addFloatingText(this->_timer->elapsed().getString(), 0, 0);
        this->_scene->addFloatingText("speed: " + std::to_string(this->_getSpeed()), 0, 30);
        this->_scene->addFloatingText("script: " + this->_getCurrentScriptName(), 0, 60);

        // prepare actions 
        for (auto currEvent = todo.begin(); currEvent != todo.end();) {
            if (this->_timer->elapsed() >= (*(*currEvent)->start - timeForPrepare)) {
                this->_findAndHandle(*currEvent, Operation::prepare);

                ready.push_back(*currEvent);
                currEvent = todo.erase(currEvent);
            }
            else {
                ++currEvent;
            }
        }

        if (firstPrepare) {
            this->_timer->reset();
            firstPrepare = false;
        }

        // start actions
        for (auto currEvent = ready.begin(); currEvent != ready.end();) {
            if (this->_timer->elapsed() >= *(*currEvent)->start) {
                this->_findAndHandle(*currEvent, Operation::start);

                if ((*currEvent)->end != nullptr) {
                    inprogres.push_back(*currEvent);
                }
                currEvent = ready.erase(currEvent);
            }
            else {
                ++currEvent;
            }
        }

        SDL_UnlockMutex(this->_eventMutex);
        this->_scene->draw();
        SDL_LockMutex(this->_eventMutex);

        // loop and end actions
        for (auto currEvent = inprogres.begin(); currEvent != inprogres.end();) {

            if (this->_timer->elapsed() >= (*(*currEvent)->end + extraTime)) {
                this->_findAndHandle(*currEvent, Operation::end);

                currEvent = inprogres.erase(currEvent);
            }
            else {
                this->_findAndHandle(*currEvent, Operation::loop);
                this->_handleControl(quit, isOkayToSkip, setSELECT, *currEvent, extraCommand, pause, inprogres);
                
                ++currEvent;
            }
        }

        SDL_UnlockMutex(this->_eventMutex);

        // fps stuff
        frameEndTime = SDL_GetTicks();
        frameTime = frameEndTime - frameStartTime;

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay(1000 / TARGET_FPS - frameTime);
        }

        // if all vector with events are empty, end playing script
        if (todo.empty() && ready.empty() && inprogres.empty()) {
            quit = true;
        }

        if (extraCommand != -1) {
            return extraCommand;
        }

        this->_scene->clear(Scene::Clear::floatingTextes);
    }

    this->_scene->clear(Scene::Clear::allExceptTextAndIndex);
    this->_scene->clear(Scene::Clear::text);

    return this->_findNextScrpitId(this->_gameContext->getLastScriptName(), this->_scene->getPathOption());
}

bool Game::_loadJumps()
{
    std::fstream file;
    file.open(this->_init.debugString + this->_init.linkToJump, std::ios::in | std::ios::binary);

    if (!file.good()) {
        printf("ERROR - unable to read jumps\n");
        return false;
    }

    uint32_t buffNum = 0x0000; char buffC[255];

    // check header
    this->_wipeCharArr(buffC, 255);
    file.read(reinterpret_cast<char*>(&buffC), 6);

    if (strncmp(buffC, "BEsrcF", 6) != 0) {
        printf("ERROR - unable to read jumps | header error\n");
        file.close();
        return false;
    }

    while (!file.eof()) {
        Jump* jump = new Jump;
        
        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->id = buffNum;
        
        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->routeId = buffNum;
        
        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        this->_wipeCharArr(buffC, 255);
        file.read(reinterpret_cast<char*>(buffC), buffNum);
        jump->scriptName = std::string(buffC);

        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->jumpToId = buffNum;

        this->_jumps.push_back(jump);

        if (file.eof()) {
            break;
        }
    }

    file.close();
    return true;
}

#pragma warning( "Debug string" )
bool Game::_loadInit()
{
    std::string debugString = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\";
    
    std::fstream initFile;
    initFile.open(debugString + "rose.ini", std::ios::in);

    if (!initFile.good()) {
        printf("cant open rose.ini file - ini was set to default\n");
        return false;
    }

    std::string buffs;
    std::vector<std::string> buffv;

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "debugString") {
        this->_init.debugString = buffv[1];
    }
    else {
        return false;
    }

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "startScript") {
        this->_init.startScript = buffv[1];
    }
    else {
        return false;
    }

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "linkToSys") {
        this->_init.linkToSys = buffv[1];
    }
    else {
        return false;
    }

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "windowWidth") {
        this->_init.windowWidth = std::stoi(buffv[1]);
    }
    else {
        return false;
    }

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "windowHeight") {
        this->_init.windowHeight = std::stoi(buffv[1]);
    }
    else {
        return false;
    }

    std::getline(initFile, buffs);
    buffv = this->_split(buffs, '=');
    if (buffv[0] == "linkToJump") {
        this->_init.linkToJump = buffv[1];
    }
    else {
        return false;
    }

    initFile.close();
    return true;
}

void Game::_setDefaultInit()
{
    this->_init.debugString = ".\\";
    this->_init.linkToSys = ".\\data\\systemList.rose";
    this->_init.startScript = "00\\00-00-A00";
    this->_init.windowWidth = 1280;
    this->_init.windowHeight = 720;
    this->_init.linkToJump = ".\\data\\boatJumps.rosej";
}

void Game::_loadClickSe()
{
    std::string path = this->_init.debugString + "SysSe\\NEWSYS\\SECLICK_01.OGG";
    auto result = this->_clickSe->load(path.c_str());
    if (result != SoLoud::SO_NO_ERROR) {
        printf("unable to load click se: %s\n", path.c_str());
    }
}

void Game::_playClickSe()
{
    this->_clickSeHandle = this->_soloud->play(*this->_clickSe);
    // add unable to play
}

void Game::_freeClickSe()
{
    delete this->_clickSe;
    this->_clickSe = NULL;
}

void Game::_speedUp()
{
    if (this->_currentSpeedLevelIndex < this->_speedLevels.size() - 1) {
        this->_currentSpeedLevelIndex++;
        this->_timer->setTimerSpeed(this->_getSpeed());
    }
}

void Game::_speedDown()
{
    if (this->_currentSpeedLevelIndex > 0) {
        this->_currentSpeedLevelIndex--;
        this->_timer->setTimerSpeed(this->_getSpeed());
    }
}

void Game::_setSpeedByIndex(int index)
{
    if (index >= 0 && index < this->_speedLevels.size()) {
        this->_currentSpeedLevelIndex = index;
    }
}

int Game::_getSpeed()
{
    return this->_speedLevels[this->_currentSpeedLevelIndex];
}

void Game::_removeFrom(Script* element, std::vector<Script*>& list)
{
    for (int i = 0; i < list.size(); i++) {
        if (element->getPath() == list[i]->getPath()) {
            list.erase(list.begin() + i);
            return;
        }
    }

    auto it = std::find(list.begin(), list.end(), element);
    if (it != list.end())
    {
        list.erase(it);
    }
}

void Game::_findAndHandle(Script::Event* event, Operation operation)
{
    switch (event->action) {
    case 0xCC01:
        if (operation == Operation::start) {
            this->_SkipFRAME_(event);
        }
        else {
            //printf("unknown operation for Skip_FRAME\n");
        }
        break;

    case 0xCC02:
        if (operation == Operation::prepare) {
            this->_PlayBgm_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_PlayBgm_Start(event);
        }
        else if (operation == Operation::end) {
            this->_PlayBgm_End(event);
        }
        else if (operation == Operation::loop) {
            this->_PlayBgm_Loop(event);
        }
        else if (operation == Operation::pause) {
            this->_PlayBgm_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_PlayBgm_Resume(event);
        }
        else {
            //printf("unknown operation for PlayBgm\n");
        }
        break;

    case 0xCC03:
        if (operation == Operation::prepare) {
            this->_CreateBG_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_CreateBG_Start(event);
        }
        else if (operation == Operation::end) {
            this->_CreateBG_End(event);
        }
        else if (operation == Operation::pause) {
            this->_CreateBG_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_CreateBG_Resume(event);
        }
        else {
            //printf("unknown operation for CreateBG\n");
        }
        break;

    case 0xCC04:
        if (operation == Operation::start) {
            this->_PrintText_Start(event);
        }
        else if (operation == Operation::end) {
            this->_PrintText_End(event);
        }
        else {
            //printf("unknown operation for PrintText\n");
        }
        break;

    case 0xCC05:
        if (operation == Operation::prepare) {
            this->_PlayVoice_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_PlayVoice_Start(event);
        }
        else if (operation == Operation::end) {
            this->_PlayVoice_End(event);
        }
        else if (operation == Operation::pause) {
            this->_PlayVoice_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_PlayVoice_Resume(event);
        }
        else {
            //printf("unknown operation for PlayVoice\n");
        }
        break;

    case 0xCC06:
        if (operation == Operation::prepare) {
            this->_PlaySe_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_PlaySe_Start(event);
        }
        else if (operation == Operation::end) {
            this->_PlaySe_End(event);
        }
        else if (operation == Operation::pause) {
            this->_PlaySe_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_PlaySe_Resume(event);
        }
        else {
            //printf("unknown operation for PlaySe\n");
        }
        break;

    case 0xCC07:
        if (operation == Operation::start) {
            this->_Next_(event);
        }
        else {
            //printf("unknown operation for Next\n");
        }
        break;

    case 0xCC08:
        if (operation == Operation::start) {
            this->_PlayMovie_Start(event);
        }
        else if (operation == Operation::end) {
            this->_PlayMovie_End(event);
        }
        else if (operation == Operation::loop) {
            this->_PlayMovie_Loop(event);
        }
        else if (operation == Operation::pause) {
            this->_PlayMovie_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_PlayMovie_Resume(event);
        }
        else {
            //printf("unknown operation for PlayMovie\n");
        }
        break;

    case 0xCC09:
        if (operation == Operation::start) {
            this->_BlackFade_Start(event);
        }
        else if (operation == Operation::end) {
            this->_BlackFade_End(event);
        }
        else {
            //printf("unknown operation for BlackFade\n");
        }
        break;

    case 0xCC0A:
        if (operation == Operation::start) {
            this->_WhiteFade_Start(event);
        }
        else if (operation == Operation::end) {
            this->_WhiteFade_End(event);
        }
        else {
            //printf("unknown operation for WhiteFade\n");
        }
        break;

    case 0xCC0B:
        if (operation == Operation::prepare) {
            this->_SetSELECT_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_SetSELECT_Start(event);
        }
        else if (operation == Operation::end) {
            this->_SetSELECT_End(event);
        }
        else {
            //printf("unknown operation for SetSELECT\n");
        }
        break;

    case 0xCC0C:
        if (operation == Operation::prepare) {
            this->_EndBGM_Prepare(event);
        }
        else if (operation == Operation::start) {
            this->_EndBGM_Start(event);
        }
        else if (operation == Operation::end) {
            this->_EndBGM_End(event);
        }
        else if (operation == Operation::pause) {
            this->_EndBGM_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_EndBGM_Resume(event);
        }
        else {
            //printf("unknown operation for PlayBgm\n");
        }
        break;

    case 0xCC0D:
        if (operation == Operation::start) {
            this->_EndRoll_Start(event);
        }
        else if (operation == Operation::end) {
            this->_EndRoll_End(event);
        }
        else if (operation == Operation::loop) {
            this->_EndRoll_Loop(event);
        }
        else if (operation == Operation::pause) {
            this->_EndRoll_Pause(event);
        }
        else if (operation == Operation::resume) {
            this->_EndRoll_Resume(event);
        }
        else {
            //printf("unknown operation for EndRoll\n");
        }
        break;

    case 0xCC0E:
        if (operation == Operation::start) {
            this->_MoveSom_Start(event);
        }
        else if (operation == Operation::end) {
            this->_MoveSom_End(event);
        }
        else if (operation == Operation::pause) {
            this->_MoveSom_End(event);
        }
        else if (operation == Operation::resume) {
            this->_MoveSom_End(event);
        }
        else {
            //printf("unknown operation for MoveSom\n");
        }
        break;

    default:
        printf("uanble to handle action: 0x%X\n", event->action);
        break;
    }
}

void Game::_handleControl(bool& quit, bool& isOkayToSkip, Script::Event* setSELECT, Script::Event* currEvent, int& extraCommand, bool& pause, std::vector<Script::Event*>& inprogres)
{
    if (this->_control.isAction()) {
        if (currEvent->action == 0xCC0B) {
            if (this->_control.check(Control::up)) {
                this->_scene->setPathOptionByIndex(0);

                SoundEffect* soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SESELECT.OGG");
                if (soundEffect) {
                    //soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
                    soundEffect->play(this->_soloud);
                }
                soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SEUP.OGG");
                if (soundEffect) {
                    //soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
                    soundEffect->play(this->_soloud);
                }

                SDL_Delay(3000);
                quit = true;
                this->_control.clear();
            }

            if (this->_control.check(Control::down)) {
                this->_scene->setPathOptionByIndex(1);

                SoundEffect* soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SESELECT.OGG");
                if (soundEffect) {
                    //soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
                    soundEffect->play(this->_soloud);
                }
                soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SEUP.OGG");
                if (soundEffect) {
                    //soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
                    soundEffect->play(this->_soloud);
                }
                SDL_Delay(3000);
                quit = true;
                this->_control.clear();
            }
        }
        
        if (this->_control.check(Control::next)) {
            this->_playClickSe();
            if (isOkayToSkip) {
                quit = true;
            }
            else {
                Script::Time timeToLoad;
                timeToLoad.second = 5;

                this->_scene->clear(Scene::Clear::allExceptTextAndIndex);
                this->_scene->clear(Scene::Clear::text);
                this->_timer->setTimerToTime(*(setSELECT)->start - timeToLoad);
            }
            this->_control.clear();
        }

        if (this->_control.check(Control::back)) {
            this->_playClickSe();
            extraCommand = Command::previousScript;
        }

        if (this->_control.check(Control::pause)) {
            this->_playClickSe();
            if (pause) {
                pause = false;
                this->_timer->resume();
                
                // resume events
                for (int i = 0; i < inprogres.size(); i++) {
                    this->_findAndHandle(inprogres[i], Operation::resume);
                }
            }
            else {
                pause = true;
                this->_timer->pause();
                
                // pause events
                for (int i = 0; i < inprogres.size(); i++) {
                    this->_findAndHandle(inprogres[i], Operation::pause);
                }
            }

            this->_control.clear();
        }

        if (this->_control.check(Control::save)) {
            // std::string name this->_scene->getInfoSave(); <- to think about this
            // this->_saveGame(std::string name);
            this->_control.clear();
        }

        if (this->_control.check(Control::save)) {
            //this->_saveGame(std::string name);
            this->_control.clear();
        }

        if (this->_control.check(Control::right)) {
            this->_speedUp();
            this->_setSpeedForEventsInProgres(inprogres);
            this->_playClickSe();
            this->_control.clear();
        }

        if (this->_control.check(Control::left)) {
            this->_speedDown();
            this->_setSpeedForEventsInProgres(inprogres);
            this->_playClickSe();
            this->_control.clear();
        }

    }
}

void Game::_setSpeedForEventsInProgres(std::vector<Script::Event*>& inprogres)
{
    // remember to add speed to video
    for (int i = 0; i < inprogres.size(); i++) {
        if (inprogres[i]->action == 0xCC02) { // bgm
            BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + inprogres[i]->data);
            if (backGoundMusic) {
                backGoundMusic->setSpeed(this->_soloud, this->_getSpeed());
            }
        }
        
        if (inprogres[i]->action == 0xCC05) { // voice
            Voice* voice = this->_gameContext->getVoice(this->_init.debugString + inprogres[i]->data + ".OGG");
            if (voice) {
                voice->setSpeed(this->_soloud, this->_getSpeed());
            }
        }

        if (inprogres[i]->action == 0xCC06) { // se
            SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + inprogres[i]->data + ".OGG");
            if (soundEffect) {
                soundEffect->setSpeed(this->_soloud, this->_getSpeed());
            }
        }
    }
}

void Game::_SkipFRAME_(Script::Event* event)
{
    // todo
}

void Game::_PlayBgm_Prepare(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->load();
    }
}

void Game::_PlayBgm_Start(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->playInt(this->_soloud);
        backGoundMusic->setSpeed(this->_soloud, this->_getSpeed());
    }
}

void Game::_PlayBgm_End(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->stop(this->_soloud);
        backGoundMusic->free();
    }
}

void Game::_PlayBgm_Loop(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        if (backGoundMusic->isReadyForLoop(this->_soloud)) {
            backGoundMusic->playLoop(this->_soloud);
        }
    }
}

void Game::_PlayBgm_Pause(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->pause(this->_soloud);
    }
}

void Game::_PlayBgm_Resume(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->resume(this->_soloud);
    }
}

void Game::_CreateBG_Prepare(Script::Event* event)
{
    BackGround* backGround = this->_gameContext->getBackGround(this->_init.debugString + event->data);
    if (backGround) {
        backGround->load();
    }
}

void Game::_CreateBG_Start(Script::Event* event)
{
    BackGround* backGround = this->_gameContext->getBackGround(this->_init.debugString + event->data);
    if (backGround) {
        this->_scene->addBackGround(backGround, 0);
    }
}

void Game::_CreateBG_End(Script::Event* event)
{
    BackGround* backGround = this->_gameContext->getBackGround(this->_init.debugString + event->data);
    if (backGround) {
        this->_scene->removeBackGround(backGround, 0);
        backGround->free();
    }
}

void Game::_CreateBG_Pause(Script::Event* event)
{
    this->_scene->pauseAnimation();
}

void Game::_CreateBG_Resume(Script::Event* event)
{
    this->_scene->resumeAnimation();
}

void Game::_PrintText_Start(Script::Event* event)
{
    this->_scene->addText(event->data);
}

void Game::_PrintText_End(Script::Event* event)
{
    this->_scene->removeText(event->data);
}

void Game::_PlayVoice_Prepare(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_init.debugString + event->data + ".OGG");
    if (voice) {
        voice->load();
    }
}

void Game::_PlayVoice_Start(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_init.debugString + event->data + ".OGG");
    if (voice) {
        voice->play(this->_soloud);
        voice->setSpeed(this->_soloud, this->_getSpeed());
    }
    
    if (event->shortName == "xxx") {
        this->_scene->setAnimationShortNameToDefalut();
    }
    else {
        BackGround* backGround = this->_scene->getLastBackGround(0);
        if (backGround) {
            backGround->tryLoadAnimation(event->shortName); // <- move to prepare?
        }
        this->_scene->setAnimationShortName(event->shortName);
    }
}

void Game::_PlayVoice_End(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_init.debugString + event->data + ".OGG");
    if (voice) {
        //this->_freeChannelsVoice.push_back(voice->getChannel());
        voice->free();
    }
    this->_scene->setAnimationShortNameToDefaultIfName(event->shortName);
}

void Game::_PlayVoice_Pause(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_init.debugString + event->data + ".OGG");
    if (voice) {
        voice->pause(this->_soloud);
    }
}

void Game::_PlayVoice_Resume(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_init.debugString + event->data + ".OGG");
    if (voice) {
        voice->resume(this->_soloud);
    }
}

void Game::_PlaySe_Prepare(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->load();
    }
}

void Game::_PlaySe_Start(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->play(this->_soloud);
        soundEffect->setSpeed(this->_soloud, this->_getSpeed());
    }
}

void Game::_PlaySe_End(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->stop(this->_soloud);
        soundEffect->free();
    }
}

void Game::_PlaySe_Pause(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->pause(this->_soloud);
    }
}

void Game::_PlaySe_Resume(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->resume(this->_soloud);
    }
}

void Game::_Next_(Script::Event* event)
{
    // todo
}

void Game::_PlayMovie_Start(Script::Event* event)
{
    this->_vDecoder->freeDecoder();
    this->_vDecoder->setPath(this->_init.debugString + event->data + ".WMV"); // <- tested for mp4 h264 its better | add video_foramt to ini file
    this->_vDecoder->start();
    if (this->_vDecoder->decodeFrame()) {
        this->_scene->addVideoFrame(this->_vDecoder->getFrame());
    }
}

void Game::_PlayMovie_End(Script::Event* event)
{
    this->_scene->clear(Scene::Clear::videoFrame);
}

void Game::_PlayMovie_Loop(Script::Event* event)
{
    if (this->_timer->isPause()) {
        return;
    }

    bool pass = false;
    for (int i = 0; i < this->_getSpeed(); i++) {
        pass = _vDecoder->decodeFrame();
    }

    if (pass) {
        this->_scene->addVideoFrame(this->_vDecoder->getFrame());
    }
}

void Game::_PlayMovie_Pause(Script::Event* event)
{
    // todo
}

void Game::_PlayMovie_Resume(Script::Event* event)
{
    // todo
}

void Game::_BlackFade_Start(Script::Event* event)
{
    // todo
}

void Game::_BlackFade_End(Script::Event* event)
{
    // todo
}

void Game::_WhiteFade_Start(Script::Event* event)
{
    // todo
}

void Game::_WhiteFade_End(Script::Event* event)
{
    // todo
}

void Game::_SetSELECT_Prepare(Script::Event* event)
{
    // test again - some of sound effect cant play at all :c
    SoundEffect* soundEffect1 = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SESELECT.OGG");
    if (soundEffect1) {
        soundEffect1->load();
    }

    SoundEffect* soundEffect2 = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SEVIEW.OGG");
    if (soundEffect2) {
        soundEffect2->load();
    }

    SoundEffect* soundEffect3 = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SEUP.OGG");
    if (soundEffect3) {
        soundEffect3->load();
    }
}

void Game::_SetSELECT_Start(Script::Event* event)
{
    auto options = this->_split(event->data, '\t');
    for (int i = 0; i < options.size(); i++) {
        this->_scene->addPathOption(options[i]);
    }

    SoundEffect* soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SEVIEW.OGG");
    if (soundEffect) {
        soundEffect->play(this->_soloud);
    }
}

void Game::_SetSELECT_End(Script::Event* event)
{   
    SoundEffect* soundEffect = this->_gameContext->getSystem()->getSoundEffect(this->_init.debugString + "SysSe\\NEWSYS\\SECANCEL_01.OGG");
    if (soundEffect) {
        soundEffect->load();
        soundEffect->play(this->_soloud);
    }

    if (!this->_scene->isPathOptionSet()) {
        this->_scene->setPathOptionByIndex(2);
    }
}

void Game::_EndBGM_Prepare(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->load();
    }
}

void Game::_EndBGM_Start(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->play(this->_soloud);
    }
}

void Game::_EndBGM_End(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->stop(this->_soloud);
        endBackGroundMusic->free();
    }
}

void Game::_EndBGM_Pause(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->pause(this->_soloud);
    }
}

void Game::_EndBGM_Resume(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->resume(this->_soloud);
    }
}

void Game::_EndRoll_Start(Script::Event* event)
{
    this->_vDecoder->freeDecoder();
    this->_vDecoder->setPath(this->_init.debugString + event->data + ".WMV"); // <- tested for mp4 h264 its better | add video_foramt to ini file
    this->_vDecoder->start();
    if (this->_vDecoder->decodeFrame()) {
        this->_scene->addVideoFrame(this->_vDecoder->getFrame());
    }
    // todo
}

void Game::_EndRoll_End(Script::Event* event)
{
    this->_scene->clear(Scene::Clear::videoFrame);
    this->_vDecoder->freeDecoder();
    // todo
}

void Game::_EndRoll_Loop(Script::Event* event)
{
    if (this->_timer->isPause()) {
        return;
    }

    if (_vDecoder->decodeFrame()) {
        this->_scene->addVideoFrame(this->_vDecoder->getFrame());
    }
}

void Game::_EndRoll_Pause(Script::Event* event)
{
    // todo
}

void Game::_EndRoll_Resume(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_Start(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_End(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_Pause(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_Resume(Script::Event* event)
{
    // todo
}

std::vector<std::string> Game::_split(std::string text, char separator)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = text.find(separator);

    while (end != std::string::npos)
    {
        result.push_back(text.substr(start, end - start));
        start = end + 1;
        end = text.find(separator, start);
    }

    result.push_back(text.substr(start));

    return result;
}

std::string Game::_findNameOfScriptById(int scriptId)
{
    for (int i = 0; i < this->_jumps.size(); i++) {
        if (this->_jumps[i]->id == scriptId) {
            return this->_jumps[i]->scriptName;
        }
    }
    return std::string();
}

std::string Game::_getCurrentScriptName()
{
    std::string path = this->_scriptHistory[this->_scriptHistory.size() - 1]->getPath();
    auto fragment = this->_split(path, '\\');
    return fragment[fragment.size() - 1];
}

int Game::_findNextScrpitId(std::string currentScriptName, int playerOption)
{
    for (int i = 0; i < this->_jumps.size(); i++) {
        if (this->_jumps[i]->scriptName == currentScriptName) {
            return this->_jumps[i + playerOption]->jumpToId;
        }
    }
    return 0;
}

void Game::_wipeCharArr(char* arr, int size)
{
    memset(arr, 0x00, size);
}
