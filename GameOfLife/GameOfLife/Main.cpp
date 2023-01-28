#include <iostream>
#include "GameOfLife.h"

int main()
{
    system("MODE CON LINES=52 COLS=201");

    GameOfLife game_of_life(500, 500, 50, 200);
    game_of_life.startTheGame(300, true);
}
