#include "gameOfCivilizationSequential.h"

using namespace std;

int main() {
    Game* seq_renderer = new SequentialGame(50, 50, "config.txt");

    seq_renderer->printGrid();
    for (int i = 0; i < 4; i++) {
        seq_renderer->advanceGame();
        cout << "Next Generation" << endl; 
        seq_renderer->printGrid();
    }
    return 0;
}