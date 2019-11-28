#ifndef __GAME_OF_CIVILIZATION_CUDA_H__
#define __GAME_OF_CIVILIZATION_CUDA_H__

#include "gameOfCivilization.h"

#include <vector>
#include <iostream>
#include <string>

class CudaGame : public Game {

private:
    int width;
    int height;
    std::vector<int> grid;
    std::vector<int> future;

    //cuda variables
    int* cudaDeviceGrid;
    int* cudaDeviceFuture;

public:

    CudaGame(int width, int height, std::string filename);
    //virtual ~SequentialRenderer();

    void setup();
    
    void printGrid();

    void advanceGame();


    //void render();


};


#endif
