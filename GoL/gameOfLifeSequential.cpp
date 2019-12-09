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

#include "gameOfLifeSequential.h"

#define IDX(X, Y) ((X) * this->width + (Y))

using namespace std;

SequentialGame::SequentialGame(int W, int H) {
    this->width = W;
    this->height = H;
    this->grid.resize(W*H);
    std::fill(this->grid.begin(), this->grid.end(), 0);
    this->future.resize(W*H);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) 
            if (i % 5 < 3 && j % 4 == 3) this->grid[IDX(i, j)] = true;
    }
}

SequentialGame::SequentialGame(int W, int H, std::string filename) {

    this->width = W;
    this->height = H;
    this->grid.resize(W*H);
    std::fill(this->grid.begin(), this->grid.end(), 0);
    this->future.resize(W*H);
    //std::fill(this->future.begin(), this->future.end(), 0);

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
            this->grid[IDX(x, y)] = true;
        }
    }
    else {
        cout << "No such file, try again." << endl;
    }
}

void
SequentialGame::printGrid() {

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            if (this->grid[IDX(i, j)] == 0)
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
    for (int l = 1; l < height - 1; l++)
    {
        for (int m = 1; m < width - 1; m++)
        {
            // finding no Of Neighbours that are alive
            int aliveNeighbours = 0;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    aliveNeighbours += (this->grid[IDX(l + i, m + j)] > 0);

            // The cell needs to be subtracted from
            // its neighbours as it was counted before
            if (this->grid[IDX(l, m)] > 0) this->grid[IDX(l, m)] = 1;
            aliveNeighbours -= this->grid[IDX(l, m)];

            // Implementing the Rules of Life

            // Cell is lonely and dies
            if ((this->grid[IDX(l, m)] == 1) && (aliveNeighbours < 2))
                this->future[IDX(l, m)] = 0;

            // Cell dies due to over population
            else if ((this->grid[IDX(l, m)] == 1) && (aliveNeighbours > 3))
                this->future[IDX(l, m)] = 0;

            // A new cell is born
            else if ((this->grid[IDX(l, m)] == 0) && (aliveNeighbours == 3))
                this->future[IDX(l, m)] = 1;

            // Remains the same
            else
                this->future[IDX(l, m)] = this->grid[IDX(l, m)];

        }
    }

    // cout << "Next Generation" << endl;
    swap(grid, future);
    // printGrid();
}
