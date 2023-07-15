#include <SDL.h>
#include <SDL_image.h>
#include "VDecoder.h"
#include <fstream>
#include <vector>
#include <string>
#include "BackGround.h"
#include "Script.h"
#include "Voice.h"
#include "SoundEffect.h"
#include "BackGroundMusic.h"
#include "GameContext.h"

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    // Inicjalizacja SDL2
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Błąd: Nie można zainicjalizować SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Inicjalizacja SDL2 Mixer
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int initFlags = Mix_Init(flags);
    if ((initFlags & flags) != flags) {
        printf("Błąd: Nie można zainicjalizować SDL Mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    // Otwarcie strumienia audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Błąd: Nie można otworzyć strumienia audio: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return -1;
    }

    // Ustawienie liczby kanałów
    Mix_AllocateChannels(16);

    SDL_Window* win = SDL_CreateWindow("School Days: Rusty Rose Edition",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    if (renderer == nullptr) {
        printf("Błąd tworzenia renderera: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }


    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, 1280, 720);

    VDecoder vdecoder = VDecoder();
    char path[] = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\vid2.mp4";
    vdecoder.setPath(path);
    vdecoder.start();

    Script script = Script("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Script.GPK~\\ENGLISH\\00\\00-00-L00.rose");
    printf("script\n");

    GameContext context = GameContext(renderer);
    context.addScript(&script);
    context.loadContentFromScripts();

    context.getVoice("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Voice00/00-00/00-00-L00/00-00-L00-0240.OGG")->play();
    context.getSoundEffect("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Se00/00-00/00-00-L00/SE00-00-L00-003.OGG")->play();

    BackGroundMusic bgm = BackGroundMusic("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\BGM/SD_BGM/sdbgm07");

    bgm.playInt();
    printf("ms int\n");
    while (!bgm.isReadyForLoop()) {
        SDL_Delay(1);
    }
    bgm.playLoop();
    printf("ms loop\n");
    
    auto bg = context.getBackGround("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\Event00/00-00/00-00-L00/00-00-L00-009");

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg->getTexture(), NULL, NULL);
    SDL_RenderPresent(renderer);
    for (int i = 0; i < 80; i++) {
        SDL_RenderCopy(renderer, bg->getNextAnimationTexture("SEK"), NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg->getTexture(), NULL, NULL);
    SDL_RenderPresent(renderer);
    for (int i = 0; i < 80; i++) {
        SDL_RenderCopy(renderer, bg->getNextAnimationTexture("MAK"), NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg->getTexture(), NULL, NULL);
    SDL_RenderPresent(renderer);
    
    context.clear();

    SDL_Delay(10000);
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

        SDL_RenderClear(renderer);

        // Renderowanie tekstury
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        // Wyświetlanie renderera na ekranie
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
