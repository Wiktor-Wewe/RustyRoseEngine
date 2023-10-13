#include <stdio.h>
#include <RustyControl.h>
#include "BackGround.h"
#include "SysImg.h"
#include "IniFile.h"
#include "SoundManager.h"
#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game game = Game();
    printf("%s", game.getStatus() ? "True" : "False");
    game.play();

    return 0;
}
