#include "gameOfCivilizationSequential.h"
#include <unistd.h>
#include "timing.h"

using namespace std;

int main() {
    SequentialGame* seq_renderer = new SequentialGame(50, 50, "config.txt");
    double totalTime = 0.0f;
    for (int i = 0; i < 50; i++) {
        seq_renderer->advanceGame();
        cout << "Generation "  << i+1 << endl; 
        Timer t;
        t.reset();
        seq_renderer->render();
        totalTime += t.elapsed();
        seq_renderer->printGrid();
        usleep(200000);
    }

    printf("Convolution time: %.6fms\n", totalTime * 1000.0f);

    return 0;
}