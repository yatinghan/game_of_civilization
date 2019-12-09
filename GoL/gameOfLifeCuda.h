#ifndef __GAME_OF_LIFE_CUDA_H__
#define __GAME_OF_LIFE_CUDA_H__

#include "gameOfLife.h"

#include <vector>
#include <iostream>
#include <string>

class CudaGame : public Game {

public:

    int width;
    int height;
    std::vector<int> grid;
    std::vector<int> future;

    //cuda variables
    int* cudaDeviceGrid;
    int* cudaDeviceFuture;

    CudaGame(int W, int H);

    CudaGame(int W, int H, std::string filename);

    //CudaGame(int W, int H, std::string filename, std::vector<int> grid2, std::vector<int> future2);
    
    void setup();
    
    void printGrid();

    void advanceGame();

    void prepPrint();

};


#endif
