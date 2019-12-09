#ifndef __GAME_OF_CIVILIZATION_CUDA_H__
#define __GAME_OF_CIVILIZATION_CUDA_H__

#include "../GoL/gameOfLifeCuda.h"
#include "gameOfCivilization.h"
#include "AreaDivisionParallel.cpp"

#include <vector>
#include <iostream>
#include <string>

class CudaGoC : public GoC {

private:

    int width;
    int height;
    vector<Tribe> tribes;
 
    //cuda variables
    int* cudaDeviceGrid;
    int* cudaDeviceFuture;

public:


    CudaGame* renderer;
    Map* map = NULL;

    CudaGoC(int width, int height);

    CudaGoC(int width, int height, std::string filename);

    void setup();

    void printGrid();

    void advanceGame();

    void render();

};


#endif
