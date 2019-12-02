#include "gameOfCivilizationCuda.h"


using namespace std;

int main() {
    Game* cuda_renderer = new CudaGame(50, 50, "config.txt");
    cuda_renderer->printGrid();
    for (int i = 0; i < 4; i++) {

        cout << "Next Generation" << endl;
        cuda_renderer->advanceGame();
        cuda_renderer->printGrid();
        // cuda_renderer->printGrid();
    }
    return 0;
}
