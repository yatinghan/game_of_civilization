#ifndef __GAME_OF_CIVILIZATION_SEQUENTIAL_H__
#define __GAME_OF_CIVILIZATION_SEQUENTIAL_H__


#include "../GoL/gameOfLifeSequential.h"
#include "gameOfCivilization.h"
#include "AreaDivision.cpp"

#include <vector>
#include <iostream>
#include <string>

class SequentialGoC : public GoC {

private:

    int width;
    int height;
    SequentialGame* renderer = NULL;
    //Map* map = NULL;

public:

    SequentialGoC(int width, int height);

    SequentialGoC(int width, int height, std::string filename);

    void printGrid();

    void advanceGame();

};


#endif
