#include "Game.h"

Game::Game()
{
    this->_gameStatus = false; 

    if (!this->_loadInit()) {
        this->_setDefaultInit();
    }

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

    // initialization of SDL2 Mixer
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int initFlags = Mix_Init(flags);
    if ((initFlags & flags) != flags) {
        printf("Error: cant initialize SDL Mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return;
    }

    // open stream audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Error: cant open audio streams: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return;
    }

    // set number of channels
    Mix_AllocateChannels(11); // <- 5 - Voice, 5 - SoundEffect, 1 - bgm

    this->_freeChannelsSoundEffect = { 1, 2, 3, 4, 5 };
    this->_freeChannelsVoice = { 6, 7, 8, 9, 10 };

    this->_window = SDL_CreateWindow("School Days: Rusty Rose Edition",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720, 0);

    this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
    if (this->_renderer == NULL) {
        printf("Error: cant create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(this->_window);
        SDL_Quit();
        return;
    }

    this->_eventMutex = SDL_CreateMutex();

    this->_gameContext = new GameContext(this->_renderer);
    this->_scene = new Scene(this->_renderer);
    this->_vDecoder = new VDecoder(this->_renderer);
    this->_timer = new Timer();

    this->_gameContext->getSystem()->setSystem(this->_init.debugString + this->_init.linkToSys);
    this->_gameContext->getSystem()->loadSystem();
    this->_scene->setFont(this->_gameContext->getSystem()->getFont());

    this->_gameStatus = true;
}

void Game::play()
{
    this->_gameContext->addScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_init.startScript + ".rose");

    while (1) {

        // if scripts is not empty, make load content for play
        if (!this->_gameContext->loadContentFromScripts()) {
            printf("Error - there is no content from script\n");
            return;
        }

        int nextScriptId = this->_playScripts();
        this->_gameContext->clear();

        this->_freeChannelsSoundEffect = { 1, 2, 3, 4, 5 };
        this->_freeChannelsVoice = { 6, 7, 8, 9, 10 };

        this->_gameContext->addScript(this->_init.debugString + "Script\\ENGLISH\\" + this->_findNameOfScriptById(nextScriptId) + ".rose");
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
    // load all events from scripts | but probably its only one in vector
    std::vector<Script::Event*> todo;
    std::vector<Script::Event*> ready;
    std::vector<Script::Event*> inprogres;

    Script::Time extraTimeToEndVoice;
    extraTimeToEndVoice.millisecond = 500;

    for (int i = 0; i < this->_gameContext->getScripts().size(); i++) {
        auto _script = this->_gameContext->getScripts()[i];
        for (int j = 0; j < _script->getEvents().size(); j++) {
            auto _event = _script->getEvents()[j];
            if (_event->action == 0xCC05) {
                *_event->end = *_event->end + extraTimeToEndVoice;
            }
            todo.push_back(_event);
        }
    }


    this->_timer->reset();

    Script::Time timeForPrepare;
    timeForPrepare.second = 2; // <- add to init file?

    Script::Time extraTime;
    extraTime.millisecond = 20;

    bool quit = false;
    SDL_Event sdl_event;

    const int TARGET_FPS = 24;
    Uint32 frameStartTime;
    Uint32 frameEndTime;
    Uint32 frameTime;

    while (!quit)
    {
        frameStartTime = SDL_GetTicks();
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_LockMutex(this->_eventMutex);

        // prepare actions 
        for (auto currEvent = todo.begin(); currEvent != todo.end();) {
            if (this->_timer->elapsed() >= (*(*currEvent)->start - timeForPrepare)) {
                this->_findAndHandle(*currEvent, Operation::prepare); // if its posible
                //printf("prepering: %s\n", todo[i]->data.c_str());

                ready.push_back(*currEvent);
                currEvent = todo.erase(currEvent);
            }
            else {
                ++currEvent;
            }
        }

        // start actions
        for (auto currEvent = ready.begin(); currEvent != ready.end();) {
            if (this->_timer->elapsed() >= *(*currEvent)->start) {
                this->_findAndHandle(*currEvent, Operation::start);
                //printf("handle start: %s\n", ready[i]->data.c_str());

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

        // end actions
        for (auto currEvent = inprogres.begin(); currEvent != inprogres.end();) {
            if (this->_timer->elapsed() >= (*(*currEvent)->end + extraTime)) {
                this->_findAndHandle(*currEvent, Operation::end);

                currEvent = inprogres.erase(currEvent);
            }
            else {
                if ((*currEvent)->action == 0xCC02) { // if init bgm start look for start loop
                    this->_playLoopWhenReadyBGM(*currEvent);
                }

                if ((*currEvent)->action == 0xCC08 || (*currEvent)->action == 0xCC0D) { // if init video start or end roll => get next frame and show
                    if (_vDecoder->decodeFrame()) {
                        this->_scene->addVideoFrame(this->_vDecoder->getFrame()); // <-  make sure that i dont skip first frame
                    }
                }
                
                ++currEvent;
            }
        }

        SDL_UnlockMutex(this->_eventMutex);

        frameEndTime = SDL_GetTicks();
        frameTime = frameEndTime - frameStartTime;

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay(1000 / TARGET_FPS - frameTime);
        }

        if (todo.empty() && ready.empty() && inprogres.empty()) {
            quit = true;
        }
    }

    this->_scene->clear(-1);
    this->_scene->clear(-2);

    return this->_findNextScrpitId(this->_gameContext->getLastScriptName(), 0);
}

bool Game::_loadJumps()
{
    std::fstream file;
    file.open(this->_init.debugString + this->_init.linkToJump, std::ios::in | std::ios::binary);

    if (!file.good()) {
        printf("ERROR - unable to read jumps\n");
        return false;
    }

    uint32_t buffNum; char buffC[255];

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

int Game::_getFirstFreeChannelSoundEffect()
{
    if (this->_freeChannelsSoundEffect.size() == 0) {
        printf("There is no free channels for sound effect, return 5");
        return 1;
    }

    int freeChannel = this->_freeChannelsSoundEffect[0];
    this->_freeChannelsSoundEffect.erase(this->_freeChannelsSoundEffect.begin());
    return freeChannel;
}

int Game::_getFirstFreeChannelVoice()
{
    if (this->_freeChannelsVoice.size() == 0) {
        printf("There is no free channels for voice, return 10");
        return 1;
    }

    int freeChannel = this->_freeChannelsVoice[0];
    this->_freeChannelsVoice.erase(this->_freeChannelsVoice.begin());
    return freeChannel;
}

void Game::_removeFrom(Script::Event* event, std::vector<Script::Event*>& list)
{
    auto it = std::find(list.begin(), list.end(), event);
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
        else {
            printf("unknown operation for PlayBgm\n");
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
        else {
            printf("unknown operation for CreateBG\n");
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
        else {
            printf("unknown operation for PlayVoice\n");
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
        else {
            printf("unknown operation for PlaySe\n");
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
        if (operation == Operation::start) {
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
            this->_PlayBgm_End(event);
        }
        else {
            printf("unknown operation for PlayBgm\n");
        }
        break;

    case 0xCC0D:
        if (operation == Operation::start) {
            this->_EndRoll_Start(event);
        }
        else if (operation == Operation::end) {
            this->_EndRoll_End(event);
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
        else {
            //printf("unknown operation for MoveSom\n");
        }
        break;

    default:
        printf("uanble to handle action: 0x%X\n", event->action);
        break;
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
        backGoundMusic->playInt();
    }
}

void Game::_PlayBgm_End(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        backGoundMusic->stop();
        backGoundMusic->free();
    }
}

void Game::_playLoopWhenReadyBGM(Script::Event* event)
{
    BackGroundMusic* backGoundMusic = this->_gameContext->getBackGroundMusic(this->_init.debugString + event->data);
    if (backGoundMusic) {
        if (backGoundMusic->isReadyForLoop()) {
            backGoundMusic->playLoop();
        }
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
        voice->setChannel(this->_getFirstFreeChannelVoice());
        voice->play();
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
        this->_freeChannelsVoice.push_back(voice->getChannel());
        voice->free();
    }
    this->_scene->setAnimationShortNameToDefaultIfName(event->shortName);
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
        soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
        soundEffect->play();
    }
}

void Game::_PlaySe_End(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_init.debugString + event->data + ".OGG");
    if (soundEffect) {
        this->_freeChannelsSoundEffect.push_back(soundEffect->getChannel());
        soundEffect->stop();
        soundEffect->free();
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
    this->_scene->clear(-3);
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

void Game::_SetSELECT_Start(Script::Event* event)
{
    // todo
    this->_scene->addText(event->data);
}

void Game::_SetSELECT_End(Script::Event* event)
{
    // todo
    this->_scene->removeText(event->data);
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
        endBackGroundMusic->play();
    }
}

void Game::_EndBGM_End(Script::Event* event)
{
    SoundEffect* endBackGroundMusic = this->_gameContext->getEndBackGroundMusic(this->_init.debugString + event->data + ".OGG");
    if (endBackGroundMusic) {
        endBackGroundMusic->stop();
        endBackGroundMusic->free();
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
    this->_scene->clear(-3);
    this->_vDecoder->freeDecoder();
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
