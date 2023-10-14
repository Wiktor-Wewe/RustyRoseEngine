#include "Game.h"
#include "BackGroundMusic.h"
#include "SoundEffect.h"
#include "Voice.h"

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
	
	this->_vDecoder = new VDecoder(this->_renderWindow->getRenderer());

	this->_timer = new Timer();

	this->_control = new RustyControl();
	
	this->_currentScript = new Script(ini->getDebugString() + ini->getMainPath() + "Script/ENGLISH/" + ini->getStartScript() + ".rose");

	if (this->_renderWindow->getInitStatus() == 0 && this->_jumps->getStatus() && this->_currentScript->isGood()) {
		this->_status = true;
	}

    this->_pauseStatus = false;
}

void Game::play()
{
    this->_control->addKeyFunction(SDLK_SPACE, [this]() { this->_pause(); });
    this->_renderWindow->getScene()->addText(("GameStatus: " + std::string(this->_status ? "True" : "False")), 0, 0, {255, 255, 255, 255}, {255, 0, 0, 255});

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

        this->_renderWindow->reversedDraw();

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

        //handleWindows(renderWindow->getManager(), &control);

        this->_control->reset();

        // fps stuff
        frameEndTime = SDL_GetTicks();
        frameTime = frameEndTime - frameStartTime;

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay(1000 / TARGET_FPS - frameTime);
        }
    }
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
    if (this->_pauseStatus) {
        this->_soundManager->resumeAll();
        this->_timer->resume();
        this->_pauseStatus = false;
    }
    else {
        this->_soundManager->pauseAll();
        this->_timer->pause();
        this->_pauseStatus = true;
    }
}

void Game::_loadEvents(EventsStateLists* eventsLists)
{ 
    for (auto currEvent = eventsLists->toLoad.begin(); currEvent != eventsLists->toLoad.end();) {
        if (this->_timer->elapsed() >= (*(*currEvent)->start - this->_timeToLoad)) {
            this->_findAndHandle(*currEvent, TaskType::load);

            eventsLists->toStart.push_back(*currEvent);
            currEvent = eventsLists->toLoad.erase(currEvent);
        }
        else {
            ++currEvent;
        }
    }
}

void Game::_startEvents(EventsStateLists* eventsLists)
{
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

void Game::_loopOrEndEvents(EventsStateLists* eventsLists)
{
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
            this->_PlayBgm_Loop(event);
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
            //this->_SetSELECT_Start(event);
        }
        else if (taskType == TaskType::loop) {
            //this->_SetSELECT_Loop(event);
        }
        else if (taskType == TaskType::end) {
            //this->_SetSELECT_End(event);
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

BackGround* Game::_getBackground(std::string path)
{
    for (auto bg : this->_backGrounds) {
        if (bg->getPath() == path) {
            return bg;
        }
    }

    return nullptr;
}

void Game::_removeBackground(std::string path)
{
    for (int i = 0; i < this->_backGrounds.size(); i++) {
        if (this->_backGrounds[i]->getPath() == path) {
            delete this->_backGrounds[i];
            this->_backGrounds.erase(this->_backGrounds.begin() + i);
            return;
        }
    }
}

void Game::_PlayBgm_Load(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    this->_soundManager->add(RRE_NormalizePath(path + event->data));
    auto bgm = (BackGroundMusic*)this->_soundManager->get(RRE_NormalizePath(path + event->data + ".ogg"));
    // add option to load specific names of BGM - ini - loop
    bgm->load();
}

void Game::_PlayBgm_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    auto bgm = (BackGroundMusic*)this->_soundManager->get(RRE_NormalizePath(path + event->data));
    bgm->play();
}

void Game::_PlayBgm_Loop(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto bgm = (BackGroundMusic*)this->_soundManager->get(RRE_NormalizePath(path + event->data));
    if (bgm->isInitDone()) {
        bgm->play();
    }
}

void Game::_PlayBgm_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    this->_soundManager->remove(RRE_NormalizePath(path + event->data));
}

void Game::_CreateBG_Load(Script::Event* event)
{
    auto renWin = this->_renderWindow;
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    auto bg = new BackGround(renWin->getRenderer(), renWin->getScreenSize(), RRE_NormalizePath(path + event->data + ".png"));
    bg->load();

    this->_backGrounds.push_back(bg);
}

void Game::_CreateBG_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    auto bg = this->_getBackground(RRE_NormalizePath(path + event->data + ".png"));
    if (bg) {
        this->_renderWindow->getScene()->addImage(event->data, bg->getTexture(), NULL);
    }
}

void Game::_CreateBG_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    this->_renderWindow->getScene()->removeImage(event->data);
    this->_removeBackground(RRE_NormalizePath(path + event->data + ".png"));
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
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto voice = (Voice*)this->_soundManager->get(RRE_NormalizePath(path + event->data + ".ogg"));
    voice->load();
}

void Game::_PlayVoice_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto voice = (Voice*)this->_soundManager->get(RRE_NormalizePath(path + event->data + ".ogg"));
    voice->play();

    if (this->_backGrounds.empty()) {
        return;
    }

    if (this->_backGrounds[0]->tryLoadAnimation(event->shortName)) {
        this->_renderWindow->getScene()->addImage(event->data, this->_backGrounds[0]->getAnimation(event->shortName), NULL, 1);
    }
}

void Game::_PlayVoice_Loop(Script::Event* event)
{
    if (this->_backGrounds.empty()) {
        return;
    }

    auto nextAnimation = this->_backGrounds[0]->getAnimation(event->shortName);
    if (nextAnimation) {
        this->_renderWindow->getScene()->removeImage(event->data);
        this->_renderWindow->getScene()->addImage(event->data, nextAnimation, NULL, 1);
    }
}

void Game::_PlayVoice_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    this->_soundManager->remove(RRE_NormalizePath(path + event->data + ".ogg"));
}

void Game::_PlaySe_Load(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto se = (SoundEffect*)this->_soundManager->get(RRE_NormalizePath(path + event->data + ".ogg"));
    se->load();
}

void Game::_PlaySe_Start(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    auto se = (SoundEffect*)this->_soundManager->get(RRE_NormalizePath(path + event->data) + ".ogg");
    se->play();
}

void Game::_PlaySe_End(Script::Event* event)
{
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();
    this->_soundManager->remove(RRE_NormalizePath(path + event->data + ".ogg"));
}

void Game::_Next_(Script::Event* event)
{
    // dont delete this function -> if delete -> error nullptr exceprion while delete scalar in string
    // mozzarella
}

void Game::_PlayMovie_Start(Script::Event* event)
{
    // i dont know if this gonna work :) to test
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    this->_vDecoder->free();
    this->_vDecoder->setPath(RRE_NormalizePath(path + event->data + ".WMV")); // <- tested for mp4 h264 its better | add video_foramt to ini file
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
    for (int i = 0; i < this->_soundManager->getCurrentSpeed(); i++) {
        pass = _vDecoder->decodeFrame();
    }

    if (pass) {
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_PlayMovie_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeImage(event->data);
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
    // same as just normal video, but at the end open save screen
    std::string path = this->_iniFile->getDebugString() + this->_iniFile->getMainPath();

    this->_vDecoder->free();
    this->_vDecoder->setPath(RRE_NormalizePath(path + event->data + ".WMV"));
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
    for (int i = 0; i < this->_soundManager->getCurrentSpeed(); i++) {
        pass = _vDecoder->decodeFrame();
    }

    if (pass) {
        this->_renderWindow->getScene()->addImage(event->data, this->_vDecoder->getFrame(), NULL);
    }
}

void Game::_EndRoll_End(Script::Event* event)
{
    this->_renderWindow->getScene()->removeImage(event->data);
}
