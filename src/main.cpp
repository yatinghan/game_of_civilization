#include "gameOfCivilizationSequential.h"
#include <unistd.h>

using namespace std;

int main() {
    Game* seq_renderer = new SequentialGame(50, 50, "config.txt");
    for (int i = 0; i < 50; i++) {
        seq_renderer->advanceGame();
        cout << "Generation "  << i+1 << endl; 
        seq_renderer->printGrid();
        usleep(300000);
    }
    return 0;
}