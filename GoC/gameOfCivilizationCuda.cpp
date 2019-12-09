
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

//include <cuda.h>
//include <cuda_runtime.h>
//include <driver_functions.h>
#include <assert.h>

#include "gameOfCivilizationCuda.h"

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32
#define BLOCK_SIZE 1024
#define IDX(X, Y) ((X) * width + (Y))

using namespace std;


CudaGoC::CudaGoC(int W, int H) {
    this->width = W;
    this->height = H;
    this->renderer = new CudaGame(W, H);
    this->map = new Map(H, W, 8, renderer->grid);
    renderer->setup();
}

CudaGoC::CudaGoC(int W, int H, std::string filename) {
    this->width = W;
    this->height = H;
    this->renderer = new CudaGame(W, H, filename);
    this->map = new Map(H, W, 8, renderer->grid);
    renderer->setup();
}

void 
CudaGoC::printGrid() {
    int tribe_index = 2;
    for (auto t : tribes) {
        for (auto p : t) renderer->grid[IDX(p.first, p.second)] = tribe_index;
        tribe_index ++;
    }

    for (int i = 0; i < this->height; i++) 
    { 
        for (int j = 0; j < this->width; j++) 
        { 
            if (renderer->grid[IDX(i, j)] == 0) 
                cout << ". "; 
            else if (renderer->grid[IDX(i, j)] == 1)
                cout << "* "; 
            else 
                cout << map->colors[(renderer->grid[IDX(i, j)]-2)%8] << "* " << RESET;
        } 
        cout << RESET << endl; 
    } 
}

void 
CudaGoC::advanceGame() {
    renderer->advanceGame();
}

void
CudaGoC::render() {
    renderer->prepPrint();
    map = new Map(this->height, this->width, 8, renderer->grid);
    tribes = map->get_tribes();
}