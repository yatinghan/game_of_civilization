#ifndef __GAME_OF_LIFE_SEQUENTIAL_H__
#define __GAME_OF_LIFE_SEQUENTIAL_H__

#include "gameOfLife.h"

#include <vector>
#include <iostream>
#include <string>

class SequentialGame : public Game {

public:

    int width;
    int height;
    std::vector<int> grid;
    std::vector<int> future;

    SequentialGame(int width, int height);

    SequentialGame(int width, int height, std::string filename);

    void printGrid();

    void advanceGame();

};


#endif
