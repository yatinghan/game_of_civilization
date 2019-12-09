#include "gameOfCivilizationCuda.h"
#include "gameOfCivilizationSequential.h"
#include "timing.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(int argc,char* argv[]) {

    int count = -1;
    int width = 50;
    int height = 50;
    int printcuda = 0;

    int opt;
    
    while ((opt = getopt(argc, argv, "h:n:w:p")) != -1) {
        switch (opt) {
            case 'h':                   
                height = atoi(optarg);
                break;
            case 'n':                   
                count = atoi(optarg);
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'p':
                printcuda = 1;
                break;
            default:
                printf("Usage: ./cudaGame -n n -w w -h h -p\n");
                return -1;
        }
    }

    if (width <= 0 || height <= 0 || count <= 0) {
        printf("Usage: ./cudaGame -n n -w w -h h -p \n");
        return -1;
    }

    Game* cuda_renderer = new CudaGame(width, height, "config.txt");

    double totalCudaTime = 0.0f;
    //cuda_renderer->printGrid();
    for (int i = 0; i < count; i++) {
        Timer t;
        t.reset();
        cuda_renderer->advanceGame();
        double simulateStepTime = t.elapsed();
        totalCudaTime += simulateStepTime;
        if (printcuda) {
            cout << "Next Generation" << endl;
            cuda_renderer->printGrid();
        }
    }
    
    printf("Total simulation time for CUDA version: %.6fms\n", totalCudaTime * 1000.0f);

    return 0;
}
