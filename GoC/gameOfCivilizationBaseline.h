#ifndef __GAME_OF_CIVILIZATION_BASELINE_H__
#define __GAME_OF_CIVILIZATION_BASELINE_H__


#include "../GoL/gameOfLifeSequential.h"
#include "gameOfCivilization.h"
#include "AreaDivisionBFS.cpp"

#include <vector>
#include <iostream>
#include <string>

class BaselineGoC : public GoC {

private:

    int width;
    int height;
    vector<Tribe> tribes;

public:

    SequentialGame* renderer;
    Map* map = NULL;

    BaselineGoC(int width, int height);

    BaselineGoC(int width, int height, std::string filename);

    //void setup();

    void printGrid();

    void advanceGame();

    void render();

};


#endif
