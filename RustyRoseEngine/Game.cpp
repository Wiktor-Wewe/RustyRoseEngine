#include "Game.h"
#include "BackGroundMusic.h"
#include "SoundEffect.h"
#include "Voice.h"
#include "Timer.h"

Game::Game()
{
	this->_status = false;
	this->_iniFile = new IniFile();
	this->_iniFile->loadFile("C:/Users/Wiktor/source/repos/RustyRoseEngine/x64/Debug/rose.ini");
	auto ini = this->_iniFile;

	this->_renderWindow = new RustyRenderWindow(ini->getTitle(), ini->getWindowWidth(), ini->getWindowHeight(), (ini->getDebugString() + ini->getPathToFont()).c_str());
	
	this->_jumps = new Jumps();
	this->_jumps->load(ini->getDebugString() + ini->getPathToJump());
	
	this->_soundManager = new SoundManager();
    this->_backgroundManager = new BackGroundManager(this->_renderWindow->getRenderer(), this->_renderWindow->getScene(), this->_renderWindow->getScreenSize());
	
	this->_vDecoder = new VDecoder(this->_renderWindow->getRenderer());

	this->_timer = new Timer();

	this->_control = new RustyControl();
	
	this->_currentScript = new Script(ini->getDebugString() + ini->getMainPath() + "Script/ENGLISH/" + ini->getStartScript() + ".rose");

	if (this->_renderWindow->getInitStatus() == 0 && this->_jumps->getStatus() && this->_currentScript->isGood()) {
		this->_status = true;
	}

    this->_timeToLoad.seconds = 1;
    this->_timeToEnd.milliseconds = 500;
    this->_eventsStateLists = nullptr;

    this->_soundManager->setGlobalSE(this->_iniFile);

    this->_pauseStatus = false;
}

void Game::play()
{
    this->_control->addKeyFunction(SDLK_d, [this]() {this->_debug(); });
    this->_control->addKeyFunction(SDLK_SPACE, [this]() { this->_pause(); });
    this->_control->addKeyFunction(SDLK_LEFT, [this]() { this->_speedDown(); });
    this->_control->addKeyFunction(SDLK_RIGHT, [this]() { this->_speedUp(); });
    this->_control->addKeyFunction(SDLK_n, [this]() { this->_next(); });
    
    this->_renderWindow->getScene()->addText(("GameStatus: " + std::string(this->_status ? "True" : "False")), 0, 0, {255, 255, 255, 255}, {255, 0, 0, 255});
    auto gameplayMenu = new RustyWindow(this->_renderWindow->getRenderer(), this->_renderWindow->getScreenSize(), this->_renderWindow->getFonts()->medium, 1000, 100);
    gameplayMenu->setBackgroundColor({ 0x00, 0x00, 0x00, 0x50 });
    gameplayMenu->setPosition((1280 / 2) - (1000 / 2), 50);
    
    gameplayMenu->addButton("PAUSE", 100, 20, 100, 30, this->_renderWindow->getFonts()->small);
    gameplayMenu->getButton(1)->setFunction([this]() -> int { return this->_pauseWindow(); });

    gameplayMenu->addButton("SPEED UP", 250, 20, 100, 30, this->_renderWindow->getFonts()->small);
    gameplayMenu->getButton(2)->setFunction([this]() -> int { return this->_speedUpWindow(); });

    gameplayMenu->addButton("SPEED DOWN", 400, 20, 100, 30, this->_renderWindow->getFonts()->small);
    gameplayMenu->getButton(3)->setFunction([this]() -> int { return this->_speedDownWindow(); });

    gameplayMenu->addButton("DEBUG", 550, 20, 100, 30, this->_renderWindow->getFonts()->small);
    gameplayMenu->getButton(4)->setFunction([this]() -> int { return this->_debugWindow(); });

    gameplayMenu->addButton("NEXT", 700, 20, 100, 30, this->_renderWindow->getFonts()->small);
    gameplayMenu->getButton(5)->setFunction([this]() -> int { return this->_nextWindow(); });

    this->_renderWindow->getManager()->addWindow(gameplayMenu);

    this->_eventsStateLists = new Script::EventsStateLists();
    for (auto event : this->_currentScript->getEvents()) {
        this->_eventsStateLists->toLoad.push_back(event);
    }
    this->_backgroundManager->setEventsStateLists(this->_eventsStateLists);

    // fps
    const int TARGET_FPS = 24; // <- no need more | video are in 24 fps
    Uint32 frameStartTime;
    Uint32 frameEndTime;
    Uint32 frameTime;

    this->_timer->reset(); // reset timer

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        frameStartTime = SDL_GetTicks(); // fps stuff
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                this->_control->handle(e);
            }
        }

        this->_loadEvents();
        this->_startEvents();

        this->_renderWindow->draw();

        this->_loopOrEndEvents();

        /*
        RRW_MouseInfo mouseInfo = this->_control->getMouseInfo();
        RRW_MouseMove mouseMove = this->_control->getMouseMove();
        this->_renderWindow->getScene()->clear(RustyScene::Clear::Dialogs);
        this->_renderWindow->getScene()->addDialog("Mouse x: " + std::to_string(mouseInfo.x));
        this->_renderWindow->getScene()->addDialog("Mouse y: " + std::to_string(mouseInfo.y));
        this->_renderWindow->getScene()->addDialog("Click Left: " + std::string(mouseInfo.clickL == true ? "True" : "False"));
        this->_renderWindow->getScene()->addDialog("Click Right: " + std::string(mouseInfo.clickR == true ? "True" : "False"));
        this->_renderWindow->getScene()->addDialog("Move X: " + std::to_string(mouseMove.vecx));
        this->_renderWindow->getScene()->addDialog("Move Y: " + std::to_string(mouseMove.vecy));
        this->_renderWindow->getScene()->addDialog("Current Window Id: " + std::to_string(this->_renderWindow->getManager()->getCurrentWindowId()));
        this->_renderWindow->getScene()->addDialog("Pause Status: " + std::string(this->_pauseStatus == true ? "True" : "False"));
        this->_renderWindow->getScene()->addDialog("Timer: " + this->_timer->elapsed().getString());
        */

        this->_handleWindows();

        this->_control->reset();

        // fps stuff
        frameEndTime = SDL_GetTicks();
        frameTime = frameEndTime - frameStartTime;

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay(1000 / TARGET_FPS - frameTime);
        }
    }

    this->_backgroundManager->free();
    delete this->_eventsStateLists;
    this->_eventsStateLists = nullptr;
}

bool Game::getStatus()
{
	return this->_status;
}

Game::~Game()
{
    // todo
}

void Game::_pause()
{
    this->_soundManager->globalSE->Click->play();

    if (this->_pauseStatus) {
        this->_backgroundManager->startAnimation();
        this->_soundManager->resumeAll();
        this->_timer->resume();
        this->_pauseStatus = false;
    }
    else {
        this->_backgroundManager->stopAnimation();
        this->_soundManager->pauseAll();
        this->_timer->pause();
        this->_pauseStatus = true;
    }
}

int Game::_pauseWindow()
{
    this->_pause();
    SDL_Delay(500);
    return 0;
}

void Game::_speedUp()
{
    this->_soundManager->globalSE->Click->play();
    this->_timer->setTimerSpeedUp();
    this->_soundManager->setSpeed(this->_timer->getSpeed());
}

int Game::_speedUpWindow()
{
    this->_speedUp();
    SDL_Delay(500);
    return 0;
}

void Game::_speedDown()
{
    this->_soundManager->globalSE->Click->play();
    this->_timer->setTimerSpeedDown();
    this->_soundManager->setSpeed(this->_timer->getSpeed());
}

int Game::_speedDownWindow()
{
    this->_speedDown();
    SDL_Delay(500);
    return 0;
}

int Game::_nextWindow()
{
    this->_next();
    SDL_Delay(500);
    return 0;
}

void Game::_next()
{
    this->_soundManager->globalSE->Click->play();
    // find setSELECT in this script
    Script::Event* e = nullptr;
    for (auto event : this->_eventsStateLists->toLoad) {
        if (event->type == Script::EventType::SetSELECT) {
            e = event;
            break;
        }
    }

    // if setSELECT event is null in this script -> revind to end
    if (!e) {
        for (auto event : this->_eventsStateLists->toLoad) {
            if (event->type == Script::EventType::SkipFRAME) {
                e = event;
                break;
            }
        }
    }

    if (!e) {
        printf("ERROR - Unable to NEXT\n");
        return;
    }

    // if setSELECT is not null -> set timer to setSELECT->start -> end all events that should be skipped
    this->_timer->setTimerToTime(*e->start);

    // end and remove skipped events from toStart
    for (auto it = this->_eventsStateLists->toStart.begin(); it != this->_eventsStateLists->toStart.end();) {
        if ((*it)->end != nullptr) {
            if ((*(*it)->end) <= this->_timer->elapsed()) {
                this->_findAndHandle(*it, TaskType::end);
                it = this->_eventsStateLists->toStart.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    // remove skipped events from toLoad
    for (auto it = this->_eventsStateLists->toLoad.begin(); it != this->_eventsStateLists->toLoad.end();) {
        if ((*it)->end != nullptr) {
            bool x = (*(*it)->end) <= this->_timer->elapsed();
            //printf("%s <= %s = %s\n", (*it)->end->getString().c_str(), this->_timer->elapsed().getString().c_str(), x == true ? "true" : "false");
            if ((*(*it)->end) <= this->_timer->elapsed()) {
                it = this->_eventsStateLists->toLoad.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    // end and remove skipped events from inProgress
    for (auto it = this->_eventsStateLists->inProgress.begin(); it != this->_eventsStateLists->inProgress.end();) {
        if ((*it)->end != nullptr) {
            if ((*(*it)->end) <= this->_timer->elapsed()) {
                this->_findAndHandle(*it, TaskType::end);
                it = this->_eventsStateLists->inProgress.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

void Game::_debug()
{
    this->_soundManager->globalSE->Click->play();
    int i = 69; // <- breakpoint
}

int Game::_debugWindow()
{
    this->_debug();
    SDL_Delay(500);
    return 0;
}

void Game::_handleWindows()
{
    auto manager = this->_renderWindow->getManager();
    auto control = this->_control;

    if (manager->isAnyWindow()) {
        auto mouseInfo = control->getMouseInfo();
        manager->getCurrentWindow()->updateSelectedId(mouseInfo.x, mouseInfo.y);

        if (mouseInfo.clickL) {
            manager->updateCurrentWindow(mouseInfo.x, mouseInfo.y);
            if (RRW_CheckMousePositionOnObject(mouseInfo.x, mouseInfo.y, manager->getCurrentWindow()->getBarPosition())) {
                auto mouseMove = control->getMouseMove();
                manager->getCurrentWindow()->move(mouseMove.vecx, mouseMove.vecy);
            }

            int response = manager->getCurrentWindow()->click();
            if (response == -1) {
                manager->removeCurrentWindow();
            }
        }
    }
}

void Game::_loadEvents()
{ 
    auto eventsLists = this->_eventsStateLists;

    for (auto currEvent = eventsLists->toLoad.begin(); currEvent != eventsLists->toLoad.end();) {
        if (this->_timer->elapsed() >= (*(*currEvent)->start - this->_timeToLoad)) {
            this->_findAndHandle(*currEvent, TaskType::load);

            eventsLists->toStart.push_back(*currEvent);
            currEvent = eventsLists->toLoad.erase(currEvent);
            break; // <- load one per frame, reduce lagas / at least i think so
        }
        else {
            ++currEvent;
        }
    }
}

void Game::_startEvents()
{
    auto eventsLists = this->_eventsStateLists;

    for (auto currEvent = eventsLists->toStart.begin(); currEvent != eventsLists->toStart.end();) {
        if (this->_timer->elapsed() >= *(*currEvent)->start) {
            this->_findAndHandle(*currEvent, TaskType::start);

            if ((*currEvent)->end != nullptr) {
                eventsLists->inProgress.push_back(*currEvent);
            }
            currEvent = eventsLists->toStart.erase(currEvent);
        }
        else {
            ++currEvent;
        }
    }
}

void Game::_loopOrEndEvents()
{
    auto eventsLists = this->_eventsStateLists;

    for (auto currEvent = eventsLists->inProgress.begin(); currEvent != eventsLists->inProgress.end();) {

        if (this->_timer->elapsed() >= (*(*currEvent)->end + this->_timeToEnd)) {
            this->_findAndHandle(*currEvent, TaskType::end);

            currEvent = eventsLists->inProgress.erase(currEvent);
        }
        else {
            this->_findAndHandle(*currEvent, TaskType::loop);
            if (eventsLists->inProgress.empty()) break; // <- i dont remember why buts it mey be important
            // prabobly just because some end event may clear inProgress list or something
            ++currEvent;
        }
    }
}

void Game::_findAndHandle(Script::Event* event, TaskType taskType)
{
    switch (event->type) {
    case Script::EventType::SkipFRAME:
        if (taskType == TaskType::start) {
            //this->_SkipFRAME_(event);
        }
        break;

    case Script::EventType::PlayBgm:
        if (taskType == TaskType::load) {
            this->_PlayBgm_Load(event);
        }
        else if (taskType == TaskType::start) {
            this->_PlayBgm_Start(event);
        }
        else if (taskType == TaskType::loop) {
            this->_PlayBgm_Loop(event);
        }
        else if (taskType == TaskType::end) {
            this->_PlayBgm_End(event);
        }
        break;

    case Script::EventType::CreateBG:
        if (taskType == TaskType::load) {
            this->_CreateBG_Load(event);
        }
        else if (taskType == TaskType::start) {
            this->_CreateBG_Start(event);
        }
        else if (taskType == TaskType::end) {
            this->_CreateBG_End(event);
        }
        break;

    case Script::EventType::PrintText:
        if (taskType == TaskType::start) {
            this->_PrintText_Start(event);
        }
        else if (taskType == TaskType::end) {
            this->_PrintText_End(event);
        }
        break;

    case Script::EventType::PlayVoice:
        if (taskType == TaskType::load) {
            this->_PlayVoice_Load(event);
        }
        else if (taskType == TaskType::start) {
            this->_PlayVoice_Start(event);
        }
        else if (taskType == TaskType::loop) {
            this->_PlayVoice_Loop(event);
        }
        else if (taskType == TaskType::end) {
            this->_PlayVoice_End(event);
        }
        break;

    case Script::EventType::PlaySe:
        if (taskType == TaskType::load) {
            this->_PlaySe_Load(event);
        }
        else if (taskType == TaskType::start) {
            this->_PlaySe_Start(event);
        }
        else if (taskType == TaskType::end) {
            this->_PlaySe_End(event);
        }
        break;

    case Script::EventType::Next:
        if (taskType == TaskType::start) {
            this->_Next_(event);
        }
        break;

    case Script::EventType::PlayMovie:
        if (taskType == TaskType::start) {
            this->_PlayMovie_Start(event);
        }
        else if (taskType == TaskType::loop) {
            this->_PlayMovie_Loop(event);
        }
        else if (taskType == TaskType::end) {
            this->_PlayMovie_End(event);
        }
        break;

    case Script::EventType::BlackFade:
        if (taskType == TaskType::start) {
            //this->_BlackFade_Start(event);
        }
        else if (taskType == TaskType::end) {
            //this->_BlackFade_End(event);
        }
        break;

    case Script::EventType::WhiteFade:
        if (taskType == TaskType::start) {
            //this->_WhiteFade_Start(event);
        }
        else if (taskType == TaskType::end) {
            //this->_WhiteFade_End(event);
        }
        break;

    case Script::EventType::SetSELECT:
        if (taskType == TaskType::start) {
            this->_SetSELECT_Start(event);
        }
        else if (taskType == TaskType::loop) {
            //this->_SetSELECT_Loop(event);
        }
        else if (taskType == TaskType::end) {
            this->_SetSELECT_End(event);
        }
        break;

    case Script::EventType::EndBGM:
        if (taskType == TaskType::load) {
            this->_EndBGM_Load(event);
        }
        else if (taskType == TaskType::start) {
            this->_EndBGM_Start(event);
        }
        else if (taskType == TaskType::end) {
            this->_EndBGM_End(event);
        }
        break;

    case Script::EventType::EndRoll:
        if (taskType == TaskType::start) {
            this->_EndRoll_Start(event);
        }
        else if (taskType == TaskType::loop) {
            this->_EndRoll_Loop(event);
        }
        else if (taskType == TaskType::end) {
            this->_EndRoll_End(event);
        }
        break;

    case Script::EventType::MoveSom:
        if (taskType == TaskType::start) {
            //this->_MoveSom_Start(event);
        }
        else if (taskType == TaskType::end) {
            //this->_MoveSom_End(event);
        }
        break;

    default:
        printf("uanble to handle action: 0x%X\n", event->type);
        break;
    }
}

void Game::_PlayBgm_Load(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data;
    auto bgm = new BackGroundMusic(path, nullptr, 0);

    this->_soundManager->add(bgm);
    bgm = (BackGroundMusic*)this->_soundManager->get(path);

    if (bgm) {
        bgm->load();
    }
}

void Game::_PlayBgm_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data;

    auto bgm = (BackGroundMusic*)this->_soundManager->get(path);
    if (bgm) {
        bgm->play();
    }
}

void Game::_PlayBgm_Loop(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data;
    auto bgm = (BackGroundMusic*)this->_soundManager->get(path);
    if (bgm && bgm->isInitDone()) {
        bgm->play();
    }
}

void Game::_PlayBgm_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data;
    this->_soundManager->remove(path);
}

void Game::_CreateBG_Load(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".PNG");
    this->_backgroundManager->setFutureBg(path);
}

void Game::_CreateBG_Start(Script::Event* event)
{
    this->_backgroundManager->swapBg();
}

void Game::_CreateBG_End(Script::Event* event)
{
    
}

void Game::_PrintText_Start(Script::Event* event)
{
    this->_renderWindow->getScene()->addDialog(event->data);
}

void Game::_PrintText_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeDialog(event->data);
}

void Game::_PlayVoice_Load(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");
    this->_soundManager->add(path);

    auto voice = (Voice*)this->_soundManager->get(path);
    if (voice) {
        voice->load();
    }
}

void Game::_PlayVoice_Start(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");

    auto voice = (Voice*)this->_soundManager->get(path);
    if (voice) {
        voice->play();
    }
    
    this->_backgroundManager->setShortName(event->shortName);
    this->_backgroundManager->startAnimation();
}

void Game::_PlayVoice_Loop(Script::Event* event)
{
    this->_backgroundManager->handleAnimation();
}

void Game::_PlayVoice_End(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");

    this->_soundManager->remove(path);
    this->_backgroundManager->resetShortName(event->shortName);
}

void Game::_PlaySe_Load(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");
    this->_soundManager->add(path);

    auto se = (SoundEffect*)this->_soundManager->get(path);
    if (se) {
        se->load();
    }
}

void Game::_PlaySe_Start(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");

    auto se = (SoundEffect*)this->_soundManager->get(path);
    if (se) {
        se->play();
    }
}

void Game::_PlaySe_End(Script::Event* event)
{
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".OGG");

    this->_soundManager->remove(path);
}

void Game::_Next_(Script::Event* event)
{
    // dont delete this function -> if delete -> error nullptr exceprion while delete scalar in string
    // mozzarella
    printf("Script End\n");
}

void Game::_PlayMovie_Start(Script::Event* event)
{
    this->_backgroundManager->resetShortName();
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".WMV");

    this->_vDecoder->free();
    this->_vDecoder->setPath(path); // <- tested for mp4 h264 its better | add video_foramt to ini file
    this->_vDecoder->start();
    if (this->_vDecoder->decodeFrame()) {
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_PlayMovie_Loop(Script::Event* event)
{
    if (this->_timer->isPause()) {
        return;
    }

    bool pass = false;
    for (int i = 0; i < this->_timer->getSpeed(); i++) {
        pass = _vDecoder->decodeFrame();
    }

    if (pass) {
        this->_renderWindow->getScene()->removeImage(event->data);
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_PlayMovie_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeImage(event->data);
}

void Game::_SetSELECT_Start(Script::Event* event)
{
    this->_renderWindow->getScene()->addDialog(event->data);
    printf("event->data: %s\n", event->data.c_str());
}

void Game::_SetSELECT_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeDialog(event->data);
}

void Game::_EndBGM_Load(Script::Event* event)
{
    // EndBGM work just like SoundEffect or Voice - so i use it like SE
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto endbgm = (SoundEffect*)this->_soundManager->get(RRE_NormalizePath(path + event->data + ".ogg"));
    endbgm->load();
}

void Game::_EndBGM_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto endbgm = (SoundEffect*)this->_soundManager->get(RRE_NormalizePath(path + event->data) + ".ogg");
    endbgm->play();
}

void Game::_EndBGM_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    this->_soundManager->remove(RRE_NormalizePath(path + event->data + ".ogg"));
}

void Game::_EndRoll_Start(Script::Event* event)
{
    this->_backgroundManager->resetShortName();
    // same as just normal video, but at the end open save screen
    std::string path = RRE_NormalizePath(this->_iniFile->getDebugString() + this->_iniFile->getMainPath() + event->data + ".WMV");

    this->_vDecoder->free();
    this->_vDecoder->setPath(path);
    this->_vDecoder->start();
    if (this->_vDecoder->decodeFrame()) {
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_EndRoll_Loop(Script::Event* event)
{
    if (this->_timer->isPause()) {
        return;
    }

    bool pass = false;
    for (int i = 0; i < this->_timer->getSpeed(); i++) {
        pass = _vDecoder->decodeFrame();
    }

    if (pass) {
        this->_renderWindow->getScene()->removeImage(event->data);
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_EndRoll_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeImage(event->data);
    // open save screen
}
