#include <iostream> 
#include <iterator> 
#include <map> 

using namespace std;

class Map {

private:

    const int** grid;
    const int M, N;
    const int range;
    vector<vector<pair<int,int>>> tribes;
    auto convolution() {}
    auto max_pooling() {}

    /* find the shortest distance to the tribe of given index */
    float distance_to_tribe(int tribe_index) {}

    /* 
     * Approximate tribe centers using convolution and max-pooling
     * For each center, add all members within range to the tribe in tribes vector
     */ 
    void init_tribes() {}

    /* Find all members of each tribe using BFS */
    void map_tribes() {}



public:

    Map(int M, int N, int** grid) : M(M), N(N) 
    {
        // allocate mem and copy grid
    }


};
