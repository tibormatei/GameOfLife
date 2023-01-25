#include <iostream>
#include "GameOfLife.h"

int main()
{
    system("MODE CON LINES=60 COLS=210");

    GameOfLife game_of_life(500, 500, 50, 200);
    game_of_life.startTheGame(2);
}
