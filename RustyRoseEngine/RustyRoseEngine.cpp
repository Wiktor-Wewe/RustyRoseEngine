#include <stdio.h>
#include "BackGroundMusic.h"
#include <RustyControl.h>
#include "Image.h"
#include "BackGround.h"
#include "SysImg.h"

int main(int argc, char* argv[]) 
{
    std::string fontPath = RRE_NormalizePath("C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\arial.ttf");
    RustyRenderWindow* renderWindow = new RustyRenderWindow("RustyRoseEngine: SchoolDays", 1280, 720, fontPath.c_str());
    RustyControl control;

    std::string shortName = "mak";

    std::string imagePath = RRE_NormalizePath("C:/Users/Wiktor/source/repos/RustyRoseEngine/x64/Debug/data/Event00\\00-00\\00-00-B00\\00-00-B00-003C" + std::string(".PNG"));
    BackGround* background = new BackGround(renderWindow->getRenderer(), renderWindow->getScreenSize(), imagePath);
    background->load();
    background->tryLoadAnimation(shortName);

    renderWindow->getScene()->addImage(background->getPath(), background->getTexture(), NULL);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                control.handle(e);
            }
        }

        renderWindow->reversedDraw();

        RRW_MouseInfo mouseInfo = control.getMouseInfo();
        RRW_MouseMove mouseMove = control.getMouseMove();
        renderWindow->getScene()->clear(RustyScene::Clear::Dialogs);
        renderWindow->getScene()->addDialog("Mouse x: " + std::to_string(mouseInfo.x));
        renderWindow->getScene()->addDialog("Mouse y: " + std::to_string(mouseInfo.y));
        renderWindow->getScene()->addDialog("Click Left: " + std::string(mouseInfo.clickL == true ? "True" : "False"));
        renderWindow->getScene()->addDialog("Click Right: " + std::string(mouseInfo.clickR == true ? "True" : "False"));
        renderWindow->getScene()->addDialog("Move X: " + std::to_string(mouseMove.vecx));
        renderWindow->getScene()->addDialog("Move Y: " + std::to_string(mouseMove.vecy));
        renderWindow->getScene()->addDialog("Current Window Id: " + std::to_string(renderWindow->getManager()->getCurrentWindowId()));

        renderWindow->getScene()->removeImage(background->getPath() + shortName + "anim");
        renderWindow->getScene()->addImage(background->getPath() + shortName + "anim", background->getAnimation("mak"), NULL);
        //handleWindows(renderWindow->getManager(), &control);

        control.resetMove();
        SDL_Delay(3);
    }

    delete renderWindow;
    delete background;
    return 0;
}
