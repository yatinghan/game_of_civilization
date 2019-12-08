#include <iostream> 
#include <iterator> 
#include <map> 
#include <vector>
#include <math.h>

#define CONV_MATRIX_SIZE 10
#define MAX_POOL_SIZE 4
#define TRIBE_MIN_POPULATION 15

using namespace std;
typedef pair<int, int> Member; // pair<row, column>
typedef vector<Member> Tribe; 

template <typename T>
struct Matrix {
    vector<T> matrix;
    int height, width;
};

class Map {

private:

    vector<int> grid;
    const int M, N; // M = grid height; N = grid width
    const int range;
    vector<Tribe> tribes;

    void printIntMatrix(Matrix<int> mat)
    {
        for (int i = 0; i < mat.height; i++) 
        { 
            for (int j = 0; j < mat.width; j++) 
            { 
                int pop = mat.matrix[i * mat.width + j];
                if (pop < 10) 
                    cout << pop << " "; 
                else 
                    cout << pop;
            } 
            cout << endl; 
        } 
    }

    void printMemberMatrix(Matrix<Member> mat, Matrix<int> value)
    {
        for (int i = 0; i < mat.height; i++) 
        { 
            for (int j = 0; j < mat.width; j++) 
            { 
                Member leader = mat.matrix[i * mat.width + j];
                int pop;
                if (leader.first == -1) pop = 0;
                else pop = value.matrix[leader.first * value.width + leader.second];
                if (pop < 10) 
                    cout << pop << " "; 
                else 
                    cout << pop;
            } 
            cout << endl; 
        } 
    }

    Matrix<int> convolution() 
    {
        int conv_result_height = M - CONV_MATRIX_SIZE + 1;
        int conv_result_width = N - CONV_MATRIX_SIZE + 1;
        vector<int> conv_result(conv_result_height * conv_result_width, 0); 
        
        for (int grid_r = 0; grid_r < conv_result_height; grid_r++) {
            for (int grid_c = 0; grid_c < conv_result_width; grid_c++) {

                int total = 0;
                for (int mat_r = 0; mat_r < CONV_MATRIX_SIZE; mat_r ++) {
                    for (int mat_c = 0; mat_c < CONV_MATRIX_SIZE; mat_c++) {
                        total += grid[(grid_r + mat_r) * N + (grid_c + mat_c)];
                    }
                }
                conv_result[grid_r * conv_result_width + grid_c] = total;
            }
        }

        Matrix<int> result;
        result.matrix = conv_result;
        result.height = conv_result_height;
        result.width = conv_result_width;
        return result;
    }

    Matrix<Member> max_pooling(vector<int> input_matrix, int input_h, int input_w) 
    {
        int pool_result_height = input_h - MAX_POOL_SIZE + 1;
        int pool_result_width = input_w - MAX_POOL_SIZE + 1;
        vector<Member> pool_result(pool_result_height * pool_result_width, pair<int, int>(-1,-1)); 
        
        for (int grid_r = 0; grid_r < pool_result_height; grid_r++) {
            for (int grid_c = 0; grid_c < pool_result_width; grid_c++) {

                int max = 0;
                Member max_coord;
                for (int mat_r = 0; mat_r < MAX_POOL_SIZE; mat_r ++) {
                    for (int mat_c = 0; mat_c < MAX_POOL_SIZE; mat_c++) {
                        int cur = input_matrix[(grid_r + mat_r) * input_w + (grid_c + mat_c)];
                        if (cur >= max) {
                            max = cur;
                            max_coord = pair<int, int>(grid_r + mat_r, grid_c + mat_c);
                        }
                    }
                }
                if (max >= TRIBE_MIN_POPULATION)
                    pool_result[grid_r * pool_result_width + grid_c] = max_coord;
            }
        }

        Matrix<Member> result; 
        result.matrix = pool_result;
        result.height = pool_result_height;
        result.width = pool_result_width;
        return result;
    }

    /* find the shortest distance to the tribe of given index */
    float distance_to_tribe(Member p, int tribe_index) 
    {
        Tribe t = tribes[tribe_index];
        float shortest_dist = INFINITY;
        for (auto member : t)
        {
            float dist = (member.first - p.first) * (member.first - p.first) + 
                         (member.second - p.second) * (member.second - p.second);
            if (dist < shortest_dist) shortest_dist = dist;
        }
        return shortest_dist;
    }

    /* 
     * Approximate tribe centers using convolution and max-pooling
     * For each center, add all members within range to the tribe in tribes vector
     */ 

    int isMemberInTribes(Member p)
    {
        // p coordinates out of bound
        bool withinBound = (0 <= p.first && p.first < M) && (0 <= p.second && p.second < N);
        if (!withinBound) return -1;

        // p is not alive
        if (grid[p.first*N+p.second] == 0) return -1;

        // search p's citizenship
        for (int t = 0; t < tribes.size(); t++) {
            for (auto m : tribes[t]) {
                if (m.first == p.first && m.second == p.second) return t;
            }
        }
        return -1;
    }

    int searchNearbyTribe(Member p)
    {
        int radius = range/2;
        for (int r = p.first - radius; r <= p.first + radius; r++) {
            for (int c = p.second - radius; c <= p.second + radius; c++) {
                Member neighbor = pair<int,int>(r,c);
                float delta_r = abs(neighbor.first - p.first);
                float delta_c = abs(neighbor.second - p.second);
                if (delta_c * delta_c + delta_r * delta_r > radius * radius) continue; // distance > radius
                int t = isMemberInTribes(neighbor);
                if (t != -1) return t;
            }
        }
        return -1;
    }

    // int searchNearbyTribe(Member p)
    // {
    //     for (int i = 0; i < tribes.size(); i++) {
    //         for (auto m : tribes[i]) {
    //             if (abs(p.first - m.first) <= 4 && abs(p.second - m.second) <= 4) {
    //                 // cout << "Member (" << p.first << "," << p.second << ")";
    //                 // cout << "found nearby tribe resident (" << m.first << "," << m.second << ")" << endl;
    //                 return i;
    //             }
    //         }
    //     }
    //     return -1;
    // }

    void init_tribes() 
    {
        auto conv = this->convolution();
        // printIntMatrix(conv);
        auto max_pool = this->max_pooling(conv.matrix, conv.height, conv.width);
        // cout << "\n\n" << endl;
        // printMemberMatrix(max_pool, conv);
        // cout << "\n\n" << endl;
        // cout << "conv.height=" << conv.height << "; conv.width=" << conv.width << endl;
        // Member lastLeader = max_pool.matrix[max_pool.matrix.size()-1];
        // cout << "leader at last max_pool matrix:" <<  lastLeader.first << "," << lastLeader.second << endl;
        // cout << "population at last conv matrix:" <<  conv.matrix[42* conv.width + 42] << endl;
        // cout << "population at this matrix:" <<  conv.matrix[lastLeader.first* conv.width + lastLeader.second] << endl;
        // cout << "max_pool.height=" << max_pool.height << "; max_pool.width=" << max_pool.width << endl;
        for (int r = 0; r < max_pool.height; r++) {
            for (int c = 0; c < max_pool.width; c++) {

                // get tribe leader from max pooling result
                pair<int, int> tribe_leader= max_pool.matrix[r * max_pool.height + c];

                // if there's no tribe leader in this area
                if (tribe_leader.first == -1) continue; 

                // collect all life in matrix starting with the coordinates of this tribe leader
                vector<Member> life_in_leader_matrix; 
                for (int tr = tribe_leader.first; tr < tribe_leader.first + CONV_MATRIX_SIZE; tr++) {
                    for (int tc = tribe_leader.second; tc < tribe_leader.second + CONV_MATRIX_SIZE; tc++) {
                        if (grid[tr * N + tc] > 0) {
                            Member p = pair<int, int>(tr, tc);
                            if (isMemberInTribes(p) != -1) continue; // already a member of some tribe
                            int nearbyT = searchNearbyTribe(p);
                            if (nearbyT == -1) // if there's no tribe nearby
                                life_in_leader_matrix.push_back(p);
                            else // found nearby existing tribe, add this life to the tribe found
                                tribes[nearbyT].push_back(p);
                        }
                    }
                }
                if (life_in_leader_matrix.size() >= TRIBE_MIN_POPULATION) 
                    // qualified to establish a new tribe
                    tribes.push_back(life_in_leader_matrix);
            }
        }
    }

    /* Find all members of each tribe using BFS */
    void map_tribes() {}



public:

    Map(int M, int N, int range, vector<int> grid) : 
        M(M), N(N), range(range), grid(grid) {}

    vector<Tribe> get_tribes() 
    {   
        tribes.clear();
        this->init_tribes();
        // int count = 0;
        // for (auto t : tribes) {
        //     cout << "Tribe " << count << ": ";
        //     for (auto l : t) cout << "(" << l.first << "," << l.second << ")  ";
        //     cout << endl;
        //     count++;
        // }
        // cout << tribes.size() << " tribes in total" << endl;
        return tribes;
    }

};
