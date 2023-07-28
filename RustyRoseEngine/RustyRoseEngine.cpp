#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game game = Game();
    if (!game.isGameGood()) {
        printf("cant initialize game\n");
        return -1;
    }

    game.play("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Script\\ENGLISH\\00\\00-00-A02.rose");

    SDL_Texture* texture = SDL_CreateTexture(game.getRenderer(), SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, 1280, 720);

    VDecoder vdecoder = VDecoder();
    char path[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\vid2.mp4";
    vdecoder.setPath(path);
    vdecoder.start();

    SDL_Delay(1000);
    while (vdecoder.decodeFrame()) {

        void* texturePixels;
        int texturePitch;
        SDL_LockTexture(texture, nullptr, &texturePixels, &texturePitch);

        // Kopiowanie danych pikseli z AVFrame do bufora pikseli tekstury
        uint8_t* yPlane = static_cast<uint8_t*>(texturePixels);
        uint8_t* uPlane = yPlane + vdecoder.getWidthFrame() * vdecoder.getHeightFrame();
        uint8_t* vPlane = uPlane + vdecoder.getWidthFrame() * vdecoder.getHeightFrame() / 4;

        // Kopiowanie danych Y
        memcpy(yPlane, vdecoder.getFrameData()[0], vdecoder.getLinesize()[0] * vdecoder.getHeightFrame());

        // Kopiowanie danych U
        memcpy(uPlane, vdecoder.getFrameData()[1], vdecoder.getLinesize()[1] * vdecoder.getHeightFrame() / 2);

        // Kopiowanie danych V
        memcpy(vPlane, vdecoder.getFrameData()[2], vdecoder.getLinesize()[2] * vdecoder.getHeightFrame() / 2);

        // Odblokowywanie tekstury
        SDL_UnlockTexture(texture);

        SDL_RenderClear(game.getRenderer());

        // Renderowanie tekstury
        SDL_RenderCopy(game.getRenderer(), texture, nullptr, nullptr);

        // Wyświetlanie renderera na ekranie
        SDL_RenderPresent(game.getRenderer());
        SDL_Delay(1);
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(game.getRenderer());
    SDL_DestroyWindow(game.getWindow());
    SDL_Quit();

    return 0;
}
