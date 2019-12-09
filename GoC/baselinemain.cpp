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
