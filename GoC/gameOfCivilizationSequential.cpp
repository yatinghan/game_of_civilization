/**
 * Sequential Game of Civilization
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "gameOfCivilizationSequential.h"

#define IDX(X, Y) ((X) * this->width + (Y)) 

using namespace std;

SequentialGoC::SequentialGoC(int W, int H) {
    this->width = W;
    this->height = H;
    this->renderer = new SequentialGame(W, H);
    //this->map = new Map(H, W, 8, renderer->grid);
}

SequentialGoC::SequentialGoC(int W, int H, std::string filename) {
    this->width = W;
    this->height = H;
    this->renderer = new SequentialGame(W, H, filename);
    //this->map = new Map(H, W, 8, renderer->grid);
}

void 
SequentialGoC::printGrid() {
    Map map = Map(this->height, this->width, 8, renderer->grid);
    auto tribes = map.get_tribes();
    int tribe_index = 2;
    for (auto t : tribes) {
        for (auto p : t) renderer->grid[IDX(p.first, p.second)] = tribe_index;
        tribe_index ++;
    }

    for (int i = 0; i < this->height; i++) 
    { 
        for (int j = 0; j < this->width; j++) 
        { 
            if (renderer->grid[IDX(i, j)] == 0) 
                cout << ". "; 
            else if (renderer->grid[IDX(i, j)] == 1)
                cout << "* "; 
            else 
                cout << map.colors[(renderer->grid[IDX(i, j)]-2)%8] << "* " << RESET;
        } 
        cout << RESET << endl; 
    } 
}

void 
SequentialGoC::advanceGame() {
    renderer->advanceGame();
}
