#include "gameOfCivilizationSequential.h"

int main() {
    Game* seq_renderer = new SequentialGame(50, 50, "config.txt");
    for (int i = 0; i < 10; i++) {
        seq_renderer->advanceGame();
    }
    return 0;
}