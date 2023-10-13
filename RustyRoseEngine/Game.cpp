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

    bool quit = false;
    SDL_Event e;
    while (!quit) {
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

        //handleWindows(renderWindow->getManager(), &control);

        this->_control->resetMove();
        SDL_Delay(2);
    }
}

bool Game::getStatus()
{
	return this->_status;
}

Game::~Game()
{

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
