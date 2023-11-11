#include <stdio.h>
#include <RustyControl.h>
#include "BackGround.h"
#include "SysImg.h"
#include "IniFile.h"
#include "SoundManager.h"
#include "Game.h"

int main(int argc, char* argv[]) 
{
    RRE_OpenConsole();
    Game game = Game();
    RRE_LogInfo(game.getStatus() ? "Game Status: True" : "Game Status: False");
    game.play();
    return 0;
}
