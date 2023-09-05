#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game* game = new Game();
    if (!game->isGameGood()) {
        printf("cant initialize game\n");
        return -1;
    }

    game->play();

    BackGround logo = BackGround(game->getRenderer(), "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\logo");
    logo.load();
    SDL_RenderCopy(game->getRenderer(), logo.getTexture(), NULL, NULL);
    SDL_RenderPresent(game->getRenderer());
    SDL_Delay(3000);
    logo.free();
    
    SDL_DestroyRenderer(game->getRenderer());
    SDL_DestroyWindow(game->getWindow());
    // game.free(); <- todo
    SDL_Quit();

    return 0;
}
