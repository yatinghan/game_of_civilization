/**
 * Sequential Game of Civilization
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>
#include <assert.h>

#include "gameOfLifeCuda.h"

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32
#define BLOCK_SIZE 1024
#define PIXEL_PER_THREAD 4
#define IDX(X, Y) ((X) * width + (Y))

using namespace std;


////////////////////////////////////////////////////////////////////////////////////////
// Putting all the cuda kernels here
///////////////////////////////////////////////////////////////////////////////////////

struct GlobalConstants {

    int width;
    int height;
    int* grid;
    int* future;

};

__constant__ GlobalConstants cuConstRendererParams;


////////////////////////////////////////////////////////////////////////////////////////

CudaGame::CudaGame(int W, int H) {
    this->width = W;
    int width = W;
    this->height = H;
    
    this->grid.resize(W*H);
    std::fill(this->grid.begin(), this->grid.end(), 0);
    this->future.resize(W*H);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) 
            if (i % 15 < 10 && j % 15 == 10) this->grid[IDX(i, j)] = true;
            // if (i % 5 < 3 && j % 4 == 3) this->grid[IDX(i, j)] = true;
    }
    setup();
}

CudaGame::CudaGame(int W, int H, std::string filename) {
    this->width = W;
    int width = W;
    this->height = H;
    
    this->grid.resize(W*H);
    std::fill(this->grid.begin(), this->grid.end(), 0);
    this->future.resize(W*H);

    ifstream readfile(filename);
    if ( readfile.is_open() )
    {
        string fileline,xx,yy;
        int x, y;

        while (getline(readfile,fileline))
        {
            stringstream ss(fileline);
            getline(ss,xx,' ');
            getline(ss,yy,' ');
            x = stoi(xx);
            y = stoi(yy);
            this->grid[IDX(x, y)] = true;
        }
        //setup();
    } 
    else {
        cout << "No such file, try again." << endl;
    }
}

// CudaGame::CudaGame(int W, int H, std::string filename, std::vector<int> grid2, std::vector<int> future2) {
//     this->width = W;
//     this->height = H;
//     this->grid = grid2;
//     this->future = future2;
//     //this->future.resize(W*H);
//     //std::fill(this->future.begin(), this->future.end(), 0);

//     ifstream readfile(filename);
//     if ( readfile.is_open() )
//     {
//         string fileline,xx,yy;
//         int x, y;

//         while (getline(readfile,fileline))
//         {
//             stringstream ss(fileline);
//             getline(ss,xx,' ');
//             getline(ss,yy,' ');
//             x = stoi(xx);
//             y = stoi(yy);
//             this->grid[IDX(x, y)] = true;
//         }
//     }
//     else {
//         cout << "No such file, try again." << endl;
//     }
// }

void
CudaGame::setup() {

    int deviceCount = 0;
    std::string name;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    printf("---------------------------------------------------------\n");
    printf("Initializing CUDA for CudaRenderer\n");
    printf("Found %d CUDA devices\n", deviceCount);

    for (int i=0; i<deviceCount; i++) {
        cudaDeviceProp deviceProps;
        cudaGetDeviceProperties(&deviceProps, i);
        name = deviceProps.name;

        printf("Device %d: %s\n", i, deviceProps.name);
        printf("   SMs:        %d\n", deviceProps.multiProcessorCount);
        printf("   Global mem: %.0f MB\n", static_cast<float>(deviceProps.totalGlobalMem) / (1024 * 1024));
        printf("   CUDA Cap:   %d.%d\n", deviceProps.major, deviceProps.minor);
    }
    printf("---------------------------------------------------------\n");

    // By this time the scene should be loaded.  Now copy all the key
    // data structures into device memory so they are accessible to
    // CUDA kernels
    //
    // See the CUDA Programmer's Guide for descriptions of
    // cudaMalloc and cudaMemcpy

    cudaMalloc(&cudaDeviceGrid, sizeof(int) * width * height);
    cudaMalloc(&cudaDeviceFuture, sizeof(int) * width * height);

    cudaMemcpy(cudaDeviceGrid, &grid[0], sizeof(int) * width * height, cudaMemcpyHostToDevice);


    // Initialize parameters in constant memory.  We didn't talk about
    // constant memory in class, but the use of read-only constant
    // memory here is an optimization over just sticking these values
    // in device global memory.  NVIDIA GPUs have a few special tricks
    // for optimizing access to constant memory.  Using global memory
    // here would have worked just as well.  See the Programmer's
    // Guide for more information about constant memory.

    GlobalConstants params;
    params.width = width;
    params.height = height;
    params.grid = cudaDeviceGrid;
    params.future = cudaDeviceFuture;

    cudaMemcpyToSymbol(cuConstRendererParams, &params, sizeof(GlobalConstants));

}

void 
CudaGame::printGrid() {
    cudaMemcpy(&(this->grid[0]),
               cudaDeviceFuture,
               sizeof(int) * width * height,
               cudaMemcpyDeviceToHost);
    //swap(grid, future);
    for (int i = 0; i < this->width; i++) 
    { 
        for (int j = 0; j < this->height; j++) 
        { 
            if (this->grid[IDX(i, j)] == 0) 
                cout << ". "; 
            else
                cout << "* "; 
        } 
        cout << endl; 
    } 
}

__global__ 
void kernelAdvanceGame() {

    int start_idx = (blockIdx.x * blockDim.x + threadIdx.x) * PIXEL_PER_THREAD;
    int w = cuConstRendererParams.width;
    int h = cuConstRendererParams.height;
    int width = w;

    for (int idx = start_idx; idx < start_idx+PIXEL_PER_THREAD && idx < w*h; idx++) {
        int l = idx / h; //row ?
        int m = idx % h; //col ?
        if (l > 0 && l < h-1 && m > 0 && m < w-1) {

            // finding no Of Neighbours that are alive 
            int aliveNeighbours = 0; 
            for (int i = -1; i <= 1; i++) 
                for (int j = -1; j <= 1; j++) 
                    aliveNeighbours += (cuConstRendererParams.grid[IDX(l + i, m + j)] > 0); 

            // The cell needs to be subtracted from 
            // its neighbours as it was counted before 
            if (cuConstRendererParams.grid[IDX(l, m)] > 0) 
                cuConstRendererParams.grid[IDX(l, m)] = 1;

            __syncthreads();

            aliveNeighbours -= cuConstRendererParams.grid[IDX(l, m)]; 

            // Implementing the Rules of Life 

            // Cell is lonely and dies 
            if ((cuConstRendererParams.grid[IDX(l, m)] == 1) && (aliveNeighbours < 2)) 
                cuConstRendererParams.future[IDX(l, m)] = 0; 

            // Cell dies due to over population 
            else if ((cuConstRendererParams.grid[IDX(l, m)] == 1) && (aliveNeighbours > 3)) 
                cuConstRendererParams.future[IDX(l, m)] = 0; 

            // A new cell is born 
            else if ((cuConstRendererParams.grid[IDX(l, m)] == 0) && (aliveNeighbours == 3)) {
                cuConstRendererParams.future[IDX(l, m)] = 1; 
            }
                
            // Remains the same 
            else
                cuConstRendererParams.future[IDX(l, m)] = cuConstRendererParams.grid[IDX(l, m)]; 
        }
    }
    
}

__global__ 
void kernelSwap() {
    int start_idx = (blockIdx.x * blockDim.x + threadIdx.x) * (PIXEL_PER_THREAD );
    int dim = cuConstRendererParams.width * cuConstRendererParams.height;
    for (int idx = start_idx; idx < start_idx+PIXEL_PER_THREAD && idx < dim; idx++) {
            cuConstRendererParams.grid[idx] = cuConstRendererParams.future[idx]; 
    } 
}

void
CudaGame::advanceGame() {

    dim3 blockDim(BLOCK_SIZE);
    int dim = this->height*this->width;
    dim3 gridDim((dim / PIXEL_PER_THREAD + blockDim.x-1) / blockDim.x);
    kernelAdvanceGame<<<gridDim, blockDim>>>();
    kernelSwap<<<gridDim, blockDim>>>();
}

void
CudaGame::prepPrint() {
    cudaMemcpy(&(this->grid[0]),
    this->cudaDeviceFuture,
    sizeof(int) * width * height,
    cudaMemcpyDeviceToHost);
}