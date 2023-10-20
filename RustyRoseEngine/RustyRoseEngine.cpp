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
    printf("%s", game.getStatus() ? "True\n" : "False\n");
    game.play();

    return 0;
}
