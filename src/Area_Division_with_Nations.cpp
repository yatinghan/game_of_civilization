#include <iostream> 
#include <iterator> 
#include <queue>
#include <vector>
#include <math.h>
#include <assert.h>

#define CONV_MATRIX_SIZE 10
#define MAX_POOL_SIZE 10
#define TRIBE_MIN_POPULATION 15
#define NATION_MIN_POPULATION 30

const string RESET       = "\033[0m";
const string BOLDBLACK   = "\033[1m\033[40m";      /* Bold Black */
const string BOLDRED     = "\033[1m\033[41m";      /* Bold Red */
const string BOLDGREEN   = "\033[1m\033[42m";      /* Bold Green */
const string BOLDYELLOW  = "\033[1m\033[43m";      /* Bold Yellow */
const string BOLDBLUE    = "\033[1m\033[44m";      /* Bold Blue */
const string BOLDMAGENTA = "\033[1m\033[45m";      /* Bold Magenta */
const string BOLDCYAN    = "\033[1m\033[46m";      /* Bold Cyan */
const string BOLDWHITE   = "\033[1m\033[47m";      /* Bold White */

using namespace std;
typedef pair<int, int> Member; // pair<row, column>
typedef pair<int, int> Point; // pair<row, column>
typedef vector<Member> Tribe; 

template <typename T>
struct Matrix {
    vector<T> matrix;
    int height, width;
};

struct Nation {
    string color;
    vector<Point> land;
    vector<Member> people;
    int years_of_history;
    int technology_index;
};

struct History {
    Matrix<int> world_map;
    vector<Nation> nations;
};

class Map {

private:

    vector<int> grid;
    const int M, N; // M = grid height; N = grid width
    const int range;
    vector<Tribe> tribes;
    vector<Nation> nations;
    History history;
    vector<int> map; // the map records the citizenship of life on every coordinate

    void printIntMatrix(Matrix<int> mat)
    {
        for (int i = 0; i < mat.height; i++) 
        { 
            for (int j = 0; j < mat.width; j++) 
            { 
                int pop = mat.matrix[i * mat.width + j];
                if (pop < 10 || pop < 0) 
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

    int getTribeMembership(Member p)
    {
        // p coordinates out of bound
        bool withinBound = (0 <= p.first && p.first < M) && (0 <= p.second && p.second < N);
        if (!withinBound) return -1;

        // p is not alive
        if (grid[p.first*N+p.second] == 0) return -1;

        int tribe_index = map[p.first * N + p.second] == tribes.size() ? -1 : map[p.first * N + p.second];
        return tribe_index;
    }

    bool isInTribe(Member p, Tribe t)
    {
        for (auto m: t) {
            if (m.first == p.first && m.second == p.second) return true;
        }
        return false;
    }

    int searchNearbyTribe(vector<Member> group_of_wanderers)
    {
        int radius = range/2;
        float shortest_distance = INFINITY;
        int closest_tribe = -1;
        for (auto p: group_of_wanderers) {
            for (int r = p.first - radius; r <= p.first + radius; r++) {
                for (int c = p.second - radius; c <= p.second + radius; c++) {
                    Member neighbor = pair<int,int>(r,c);
                    float delta_r = abs(neighbor.first - p.first);
                    float delta_c = abs(neighbor.second - p.second);
                    if (delta_c * delta_c + delta_r * delta_r > radius * radius) continue; // distance > radius
                    int t = getTribeMembership(neighbor);
                    if (t != -1) {
                        float d = distance_to_tribe(p, t);
                        if (d < shortest_distance) {
                            shortest_distance = d;
                            closest_tribe = t;
                        }
                    }
                }
            }
        }
        return closest_tribe;
    }

    void register_new_tribe(Tribe newTribe)
    {
        if (newTribe.size() >= TRIBE_MIN_POPULATION) {
            // qualified to establish a new tribe
            int tribe_index = tribes.size();
            tribes.push_back(newTribe);
        }
        else {
            // registration failed
            for (auto m : newTribe) 
                map[m.first * N + m.second] = -1;
        }
    }

    /* 
     * Approximate tribe centers using convolution and max-pooling
     * For each center, add all members within range to the tribe in tribes vector
     */ 
    void init_tribes() 
    {
        auto conv = this->convolution();
        auto max_pool = this->max_pooling(conv.matrix, conv.height, conv.width);
        for (int r = 0; r < max_pool.height; r++) {
            for (int c = 0; c < max_pool.width; c++) {

                // get tribe leader from max pooling result
                pair<int, int> tribe_leader= max_pool.matrix[r * max_pool.height + c];

                // if there's no tribe leader in this area
                if (tribe_leader.first == -1) continue; 

                // collect all life in matrix starting with the coordinates of this tribe leader
                Tribe newTribe; 
                for (int tr = tribe_leader.first; tr < tribe_leader.first + CONV_MATRIX_SIZE; tr++) {
                    for (int tc = tribe_leader.second; tc < tribe_leader.second + CONV_MATRIX_SIZE; tc++) {
                        
                        if (grid[tr * N + tc] > 0) {
                            Member p = pair<int, int>(tr, tc);

                            // if already a member of some tribe
                            if (getTribeMembership(p) != -1) continue; 

                            // if already collected
                            if (isInTribe(p, newTribe)) continue; 

                            // find all neighbors within range using BFS
                            vector<Member> valid_neighbors = BFS(p, newTribe);
                            
                            // the group looks for a nearby tribe to settle down
                            int nearbyT = searchNearbyTribe(valid_neighbors);
                            if (nearbyT == -1)  // if there's no tribe nearby
                            {
                                newTribe.insert(newTribe.end(), valid_neighbors.begin(), valid_neighbors.end());
                                for (auto n : valid_neighbors) 
                                    map[n.first * N + n.second] = tribes.size();
                            }
                            else                // found nearby existing tribe, add this group of lives to the tribe found
                            {               
                                tribes[nearbyT].insert(newTribe.end(), valid_neighbors.begin(), valid_neighbors.end());
                                for (auto n : valid_neighbors) 
                                    map[n.first * N + n.second] = nearbyT;
                            }
                        }
                    }
                }
                register_new_tribe(newTribe);
            }
        }
    }

    /* Find all members of each tribe using BFS */
    vector<Member> BFS(Member seed, Tribe newTribe) {
        // initialize queue
        vector<Member> queue;
        queue.push_back(seed);
        vector<Member> visited = newTribe;
        vector<Member> new_neighbors = {seed};

        //find neighbors
        int radius = range/2;
        while (!queue.empty())
        {
            Member p = queue.front();
            if (!isInTribe(p, visited)) {
                visited.push_back(p);
                if (!isInTribe(p, new_neighbors)) new_neighbors.push_back(p);
                for (int r = p.first - radius; r <= p.first + radius; r++) {
                    for (int c = p.second - radius; c <= p.second + radius; c++) {
                        if (r == p.first && c == p.second) continue;                           // neighbor is p itself
                        bool withinBound = (0 <= r && r < M) && (0 <= c && c < N);
                        if (!withinBound) continue;                                            // neighbor is out of bound
                        if (grid[r*N+c] == 0) continue;                                        // neighbor is not alive

                        Member neighbor = pair<int,int>(r,c);
                        if (getTribeMembership(neighbor) != -1) continue;                      // neighbor is in other tribe
                        else if (isInTribe(neighbor, visited)) continue;                       // neighbor is already visited
                        else if (isInTribe(neighbor, queue)) continue;                         // neighbor is already in queue

                        float delta_r = abs(neighbor.first - p.first);
                        float delta_c = abs(neighbor.second - p.second);
                        if (delta_c * delta_c + delta_r * delta_r > radius * radius) continue; // neighbor is too far: distance > radius
                        
                        queue.push_back(neighbor);
                    }
                }
            }
            queue.erase(queue.begin());
        }
        return new_neighbors;
    }



public:

    vector<string> colors = {BOLDYELLOW, BOLDGREEN, BOLDBLUE, BOLDRED, BOLDMAGENTA, BOLDCYAN, BOLDBLACK, BOLDWHITE};

    Map(int M, int N, int range, vector<int> grid, History history) : 
        M(M), N(N), range(range), grid(grid), history(history)
    {
        vector<int> empty(grid.size(), -1); 
        map = empty;
    }

    vector<Tribe> get_tribes() 
    {   
        tribes.clear();
        this->init_tribes();
        int count = 0;
        for (auto t : tribes) {
            cout << "Tribe " << count << ": " << t.size() << endl;
            // for (auto l : t) cout << "(" << l.first << "," << l.second << ")  ";
            // cout << endl;
            count++;
        }
        // cout << tribes.size() << " tribes in total" << endl;
        return tribes;
    }

    vector<Nation> get_nations()
    {

    }

    void init_map()
    {

    }

    void war()
    {

    }

    /* 
     * print map with nations and tribes 
     * 0 = barren; 1 = barbarian; 2+ = nation/tribe index
     */
    static void print_map(Matrix<int> map)
    {
        vector<string>colors = {BOLDYELLOW, BOLDGREEN, BOLDBLUE, BOLDRED, BOLDMAGENTA, BOLDCYAN, BOLDBLACK, BOLDWHITE};

        for (int r = 0; r < map.height; r++) 
        { 
            for (int c = 0; c < map.width; c++) 
            { 
                if (map.matrix[r*map.width+c] == 0) 
                    cout << ". "; 
                else if (map.matrix[r*map.width+c] == 1)
                    cout << "* "; 
                else 
                    cout << colors[(map.matrix[r*map.width+c]-2)%8] << "* " << RESET;
            } 
            cout << RESET << endl; 
        }
    }

    /* convert Map to History */
    History write_history()
    {
        History history;

        Matrix<int> map_matrix;
        map_matrix.height = M; 
        map_matrix.width = N;
        map_matrix.matrix = map;

        history.world_map = map_matrix;
        history.nations = nations;
        return history;
    }

};
