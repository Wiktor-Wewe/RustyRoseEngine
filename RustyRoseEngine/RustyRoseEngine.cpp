#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game game = Game();
    if (!game.isGameGood()) {
        printf("cant initialize game\n");
        return -1;
    }

    game.play("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Script\\ENGLISH\\00\\00-00-A00.rose");

    VDecoder vdecoder = VDecoder(game.getRenderer());
    std::string path = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Movie00\\00-00\\00-00-K00\\00-00-K00-001.WMV";
    vdecoder.setPath(path.c_str());
    vdecoder.start();

    SDL_Delay(1000);
    while (vdecoder.decodeFrame()) {

        SDL_RenderClear(game.getRenderer());

        SDL_RenderCopy(game.getRenderer(), vdecoder.getFrame(), NULL, NULL);

        SDL_RenderPresent(game.getRenderer());
        SDL_Delay(1);
    }
    
    SDL_DestroyRenderer(game.getRenderer());
    SDL_DestroyWindow(game.getWindow());
    // game.free(); <- todo
    SDL_Quit();

    return 0;
}
