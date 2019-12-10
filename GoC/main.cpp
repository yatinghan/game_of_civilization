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

    SequentialGoC* cuda_renderer = new SequentialGoC(width, height);
    //SequentialGoC* cuda_renderer = new SequentialGoC(width, height, "config.txt");

    double totalCudaTime = 0.0f;
    double totalSimTime = 0.0f;
    double convolutionTime = 0.0f;
    double maxPoolingTime = 0.0f;
    double searchNearbyTribeTime = 0.0f;
    double bfsTime = 0.0f;
    double initTime = 0.0f;


    for (int i = 0; i < count; i++) {
        Timer t;
        t.reset();
        Timer t2;
        t2.reset();
        cuda_renderer->advanceGame();
        totalCudaTime += t2.elapsed();
        cuda_renderer->render();

        convolutionTime += cuda_renderer->map->convolutionTime;
        maxPoolingTime += cuda_renderer->map->maxPoolingTime;
        bfsTime += cuda_renderer->map->bfsTime;
        searchNearbyTribeTime += cuda_renderer->map->searchNearbyTribeTime;
        initTime += cuda_renderer->map->initTime;

        totalSimTime += t.elapsed();
        if (printcuda) {
            cout << "Next Generation" << endl;
            cuda_renderer->printGrid();
        }
        usleep(200000);
    }
    
    printf("Convolution time: %.6fms\n", convolutionTime * 1000.0f);
    printf("Min pooling time: %.6fms\n", maxPoolingTime * 1000.0f);
    printf("BFS time: %.6fms\n", bfsTime * 1000.0f);
    printf("Search nearby neighbor time: %.6fms\n", searchNearbyTribeTime * 1000.0f);
    printf("Total classification time: %.6fms\n", initTime * 1000.0f);
    printf("Total GoL time: %.6fms\n", totalCudaTime * 1000.0f);
    printf("Total simulation time for SEQUENTIAL version: %.6fms\n", totalSimTime * 1000.0f);

    return 0;
}
