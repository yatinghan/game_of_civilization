#ifndef __GAME_OF_CIVILIZATION_SEQUENTIAL_H__
#define __GAME_OF_CIVILIZATION_SEQUENTIAL_H__

#include "gameOfCivilization.h"
#include "Area_Division_with_Nations.cpp"

#include <vector>
#include <iostream>
#include <string>

class SequentialGame : public Game {

private:

    int width;
    int height;
    std::vector<int> grid;
    std::vector<int> future;
    History history;

public:

    SequentialGame(int width, int height, std::string filename);
    //virtual ~SequentialRenderer();

    void printGrid();

    void advanceGame();

    //void render();


};


#endif