#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
// #include "Area_Division.cpp"

#define M 50
#define N 50
using namespace std;

class GameOfLife
{
private:
	// Intiliazing the grid. 
	int grid[M][N] = {{0}};
	int future[M][N] = {{0}};
	string filename = "config.txt";

	void init_grid_from_file()
	{
		ifstream readfile(this->filename);
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

	void print_grid()
	{
		for (int i = 0; i < M; i++) 
		{ 
			for (int j = 0; j < N; j++) 
			{ 
				if (this->grid[i][j] == 0) 
					cout << ". "; 
				else
					cout << "* "; 
			} 
			cout << endl; 
		} 
	}
	
public:
    // Constructor
    GameOfLife(){};

	void Run()
	{ 	
		init_grid_from_file();
		// Displaying the grid 
		cout << "Original Generation" << endl; 
		print_grid();
		cout << endl; 
		cout << "Start game? (y/n)" << endl;
		string start;
		cin >> start;
		if (start == "y")
		{
			for (int i = 0; i < 100; i++)
			{
				system("clear");
				next_generation(); 
				usleep(200000);
			}
		}
	} 

	// Function to print next generation 
	void next_generation() 
	{

		// Loop through every cell 
		for (int l = 1; l < M - 1; l++) 
		{ 
			for (int m = 1; m < N - 1; m++) 
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
		print_grid();
	} 

};

int main(int argc, char** argv)
{
    auto game = GameOfLife();
    game.Run();
}
