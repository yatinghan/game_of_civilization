#include "gameOfLifeCuda.h"
#include "gameOfLifeSequential.h"
#include "timing.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(int argc,char* argv[]) {

    int count = -1;
    int width = 50;
    int height = 50;
    int printseq = 0;
    int printcuda = 0;

    int opt;
    
    while ((opt = getopt(argc, argv, "h:n:w:sc")) != -1) {
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
            case 's':
                printseq = 1;
                break;
            case 'c':
                printcuda = 1;
                break;
            default:
                printf("Usage: ./cudaGame -n n -w w -h h -s -c\n");
                return -1;
        }
    }

    if (width <= 0 || height <= 0 || count <= 0) {
        printf("Usage: ./cudaGame -n n -w w -h h -s -c \n");
        return -1;
    }

    Game* seq_renderer = new SequentialGame(width, height, "config.txt");

    double totalSeqTime = 0.0f;
    //seq_renderer->printGrid();
    for (int i = 0; i < count; i++) {
        Timer t;
        t.reset();
        seq_renderer->advanceGame();
        double simulateStepTime = t.elapsed();
        totalSeqTime += simulateStepTime;
        if (printseq && i > 40) {
            cout << "Next Generation" << endl; 
            seq_renderer->printGrid();
        }
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

    printf("Total simulation time for sequential version: %.6fms\n", totalSeqTime);
    printf("Total simulation time for CUDA version: %.6fms\n", totalCudaTime);

    return 0;
}
