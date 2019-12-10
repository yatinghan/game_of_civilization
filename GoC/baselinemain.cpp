#include "gameOfCivilizationBaseline.h"
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
    
    while ((opt = getopt(argc, argv, "n:d:p")) != -1) {
        switch (opt) {
            case 'n':                   
                count = atoi(optarg);
                break;
            case 'd':
                width = atoi(optarg);
                height = atoi(optarg);
                break;
            case 'p':
                printcuda = 1;
                break;
            default:
                printf("Usage: ./baselineGame -n n -d d -p\n");
                return -1;
        }
    }

    if (width <= 0 || height <= 0 || count <= 0) {
        printf("Usage: ./baselineGame -n n -d d -p\n");
        return -1;
    }

    //BaselineGoC* cuda_renderer = new BaselineGoC(width, height, "config.txt");
    BaselineGoC* cuda_renderer = new BaselineGoC(width, height);

    double totalCudaTime = 0.0f;
    double totalSimTime = 0.0f;
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

        bfsTime += cuda_renderer->map->bfsTime;
        initTime += cuda_renderer->map->initTime;

        totalSimTime += t.elapsed();
        if (printcuda) {
            cout << "Next Generation" << endl;
            cuda_renderer->printGrid();
        }
        usleep(200000);
    }
    
    printf("BFS time: %.6fms\n", bfsTime * 1000.0f);
    printf("Total classification time: %.6fms\n", initTime * 1000.0f);
    printf("Total GoL time: %.6fms\n", totalCudaTime * 1000.0f);
    printf("Total simulation time for BASELINE version: %.6fms\n", totalSimTime * 1000.0f);

    return 0;
}
