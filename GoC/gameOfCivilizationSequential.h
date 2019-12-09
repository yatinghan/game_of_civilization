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
    vector<Tribe> tribes;

public:

    SequentialGame* renderer;
    Map* map = NULL;

    SequentialGoC(int width, int height);

    SequentialGoC(int width, int height, std::string filename);

    //void setup();

    void printGrid();

    void advanceGame();

    void render();

};


#endif
