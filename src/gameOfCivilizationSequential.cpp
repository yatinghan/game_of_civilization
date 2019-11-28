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
#include <string>

#include "gameOfCivilizationSequential.h"

using namespace std;

SequentialGame::SequentialGame(int W, int H, std::string filename) {

    this->width = W;
    this->height = H;
    this->grid.resize(W);
    for (int i = 0; i < W; i++) {
        grid[i].resize(H);
    }
    this->future.resize(W);
    for (int i = 0; i < W; i++) {
        future[i].resize(H);
    }

    ifstream readfile(filename);
    if ( readfile.is_open() )
    {
        string fileline,xx,yy;
        int x, y;

        while (getline(readfile,fileline))
        {
            stringstream ss(fileline);
            getline(ss,xx,' ');
            getline(ss,yy,' ');
            x = stoi(xx);
            y = stoi(yy);
            this->grid[x][y] = true;
        }
    } 
    else {
        cout << "No such file, try again." << endl;
    }
}

void
SequentialGame::printGrid() {
    for (int i = 0; i < this->width; i++) 
    { 
        for (int j = 0; j < this->height; j++) 
        { 
            if (this->grid[i][j] == 0) 
                cout << ". "; 
            else
                cout << "* "; 
        } 
        cout << endl; 
    } 
}

void 
SequentialGame::advanceGame()  {
    // Loop through every cell 
    for (int l = 1; l < width - 1; l++) 
    { 
        for (int m = 1; m < height - 1; m++) 
        { 
            // finding no Of Neighbours that are alive 
            int aliveNeighbours = 0; 
            for (int i = -1; i <= 1; i++) 
                for (int j = -1; j <= 1; j++) 
                    aliveNeighbours += this->grid[l + i][m + j]; 

            // The cell needs to be subtracted from 
            // its neighbours as it was counted before 
            aliveNeighbours -= this->grid[l][m]; 

            // Implementing the Rules of Life 

            // Cell is lonely and dies 
            if ((this->grid[l][m] == 1) && (aliveNeighbours < 2)) 
                this->future[l][m] = 0; 

            // Cell dies due to over population 
            else if ((this->grid[l][m] == 1) && (aliveNeighbours > 3)) 
                this->future[l][m] = 0; 

            // A new cell is born 
            else if ((this->grid[l][m] == 0) && (aliveNeighbours == 3)) 
                this->future[l][m] = 1; 

            // Remains the same 
            else
                this->future[l][m] = this->grid[l][m]; 
        } 
    } 

    cout << "Next Generation" << endl; 
    swap(grid, future);
    printGrid();
}
