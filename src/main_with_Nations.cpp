#include "gameOfCivilizationSequential_with_Nations.h"
#include <unistd.h>

using namespace std;

int main() {
    Game* seq_renderer = new SequentialGame(50, 50, "config.txt");
    for (int i = 0; i < 800; i++) {
        cout << "Generation "  << i+1 << endl; 
        seq_renderer->advanceGame();
        usleep(100000);
    }
    return 0;
}