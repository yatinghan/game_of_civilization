// #include "gameOfCivilizationSequential.h"
// #include <unistd.h>

// using namespace std;

// int main() {
//     GoC* seq_renderer = new SequentialGoC(50, 50, "config.txt");
//     for (int i = 0; i < 50; i++) {
//         seq_renderer->advanceGame();
//         cout << "Generation "  << i+1 << endl; 
//         seq_renderer->printGrid();
        
//     }
//     return 0;
// }


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
    int printseq = 0;

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
                printseq = 1;
                break;
            default:
                printf("Usage: ./cudaGame -n n -w w -h h -p\n");
                return -1;
        }
    }

    if (width <= 0 || height <= 0 || count <= 0) {
        printf("Usage: ./cudaGame -n n -w w -h h -p\n");
        return -1;
    }

    GoC* seq_renderer = new SequentialGoC(width, height, "config.txt");

    double totalSeqTime = 0.0f;
    //seq_renderer->printGrid();
    for (int i = 0; i < count; i++) {
        Timer t;
        t.reset();
        seq_renderer->advanceGame();
        double simulateStepTime = t.elapsed();
        totalSeqTime += simulateStepTime;
        if (printseq) {
            cout << "Next Generation" << endl; 
            seq_renderer->printGrid();
        }
        usleep(200000);
    }

    printf("Total simulation time for sequential version: %.6fms\n", totalSeqTime*1000.0f);

    return 0;
}
