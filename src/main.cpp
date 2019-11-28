#include "gameOfCivilizationSequential.h"


using namespace std;

int main() {
    Game* seq_renderer = new SequentialGame(50, 50, "config.txt");
    for (int i = 0; i < 10; i++) {
        seq_renderer->advanceGame();
        cout << "Next Generation" << endl; 
        seq_renderer->printGrid();
    }
    return 0;
}