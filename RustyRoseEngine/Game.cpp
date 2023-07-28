#include "Game.h"

Game::Game()
{
    this->_initStatus = false;

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
    Mix_AllocateChannels(16);

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

    this->_gameContext = new GameContext(this->_renderer);
    this->_scene = new Scene(this->_renderer);
    this->_vDecoder = new VDecoder();
    this->_timer = new Timer();

    // this should looks like: 
    // this->_gameContext->getSyste()->setSyste(init.linkToSys);
    std::string linkToSys = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\systemList.rose";
    this->_gameContext->getSystem()->setSystem(linkToSys);
    this->_gameContext->getSystem()->loadSystem();
    this->_scene->setFont(this->_gameContext->getSystem()->getFont());

    this->_initStatus = true;
}

void Game::play(std::string scriptPath)
{
    this->_gameContext->addScript(scriptPath);

    if (!this->_gameContext->loadContentFromScripts()) {
        printf("Error - there is no content from script\n");
        return;
    }

    bool end = false;
    std::vector<Script::Event*> todo = this->_gameContext->getScript(scriptPath)->getEvents();
    std::vector<Script::Event*> inprogres;

    this->_timer->reset();


    bool quit = false;
    SDL_Event sdl_event;

    while (!quit)
    {
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        for (int i = 0; i < todo.size(); i++) {
            if (this->_timer->elapsed() >= *todo[i]->start) {
                // init action
                //printf("init action: 0x%X - data: %s\n", todo[i]->action, todo[i]->data.c_str());
                this->_findAndHandle(todo[i], 0);

                if (todo[i]->end != nullptr) {
                    inprogres.push_back(todo[i]);
                }
                this->_removeFrom(todo[i], todo);
            }
        }

        // draw is between init and end becouse i want to draw all texture 
        this->_scene->draw();

        for (int i = 0; i < inprogres.size(); i++) {
            if (this->_timer->elapsed() >= *inprogres[i]->end) {
                // end action
                //printf("end action: 0x%X - data: %s\n", inprogres[i]->action, inprogres[i]->data.c_str());
                this->_findAndHandle(inprogres[i], 1);

                this->_removeFrom(inprogres[i], inprogres);
            }
        }


        if (todo.empty() && inprogres.empty()) {
            end = true;
        }
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
    return this->_initStatus;
}

void Game::_removeFrom(Script::Event* event, std::vector<Script::Event*>& list)
{
    auto it = std::find(list.begin(), list.end(), event);
    if (it != list.end())
    {
        list.erase(it);
    }
}

void Game::_findAndHandle(Script::Event* event, int operation)
{
    // operation [0] = init, [1] = end, [other] = nothing yet?
    switch (event->action) {
    case 0xCC04:
        if (operation == 0) {
            this->_PrintText_Init(event);
        }
        if (operation == 1) {
            this->_PrintText_End(event);
        }
        break;
    default:
        //printf("uanble to handle action: 0x%X\n", event->action);
        break;
    }
}

void Game::_PrintText_Init(Script::Event* event)
{
    this->_scene->addText(event->data);
}

void Game::_PrintText_End(Script::Event* event)
{
    this->_scene->removeText(event->data);
}
