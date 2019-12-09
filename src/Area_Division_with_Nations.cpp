#include <iostream> 
#include <iterator> 
#include <queue>
#include <vector>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>

#define CONV_MATRIX_SIZE 10
#define MAX_POOL_SIZE 10
#define TRIBE_MIN_POPULATION 15
#define NATION_MIN_POPULATION 30

#define RESET   "\033[0m"
#define BOLDBLACK   "\033[1m\033[40m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[41m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[42m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[43m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[44m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[45m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[46m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[47m"      /* Bold White */

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
    Matrix<int> world_map; // nations only; tribes not painted
    vector<Nation> nations;
};

class Map {

private:

    vector<int> grid; // the grid records life or death of each cell
    vector<int> map; // the map records the citizenship/tribe membership of each cell
    const int M, N; // M = grid height; N = grid width
    const int range;
    vector<Tribe> tribes;
    vector<Nation> nations;
    History history; // history map only records the territory of nations

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

    /* find the shortest distance to the nation of given index */
    float distance_to_nation(Member p, int nation_index) 
    {
        Nation n = nations[nation_index];
        float shortest_dist = INFINITY;
        Member closest;
        for (auto member : n.people)
        {
            float dist = (member.first - p.first) * (member.first - p.first) + 
                         (member.second - p.second) * (member.second - p.second);
            if (dist < shortest_dist) {
                shortest_dist = dist;
                closest = member;
            }
        }
        cout << "(" << p.first << ", " << p.second;
        cout << ") shortest distance to " << nation_index;
        cout << " is " << shortest_dist;
        cout << " from (" << closest.first << ", " << closest.second << ")";
        cout << " who is from " << map[closest.first * N + closest.second] << endl;
        return shortest_dist;
    }

    int whose_closer(Member p, int a, int b)
    {
        float distA = distance_to_nation(p, a);
        float distB = distance_to_nation(p, b);
        int closer = (distB < distA) ? b : a;
        return closer;
    }

    int getTribeMembership(Member p)
    {
        // p coordinates out of bound
        bool withinBound = (0 <= p.first && p.first < M) && (0 <= p.second && p.second < N);
        if (!withinBound) return -1;

        // p is not alive
        if (grid[p.first*N+p.second] == 0) return -1;

        int tribe_index = map[p.first * N + p.second];
        if (tribe_index == -1 || tribe_index < nations.size() || tribe_index >= nations.size() + tribes.size())
            return -1;
        // return tribe index without offset
        return tribe_index - nations.size();
    }

    int getCitizenship(Member p)
    {
        // p coordinates out of bound
        bool withinBound = (0 <= p.first && p.first < M) && (0 <= p.second && p.second < N);
        if (!withinBound) return -1;

        // p is not alive
        if (grid[p.first*N+p.second] == 0) return -1;

        int nation_index = map[p.first * N + p.second];
        if (nation_index == -1 || nation_index >= nations.size()) return -1;
        else return nation_index;
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

    void init_tribes_BFS_only() 
    {
        for (int r = 0; r < M; r++) {
            for (int c = 0; c < N; c++) {

                // if no life in location (r,c)
                if (grid[r * N + c] == 0) continue; 

                // else, appoint this life as the tribe leader 
                pair<int, int> tribe_leader= pair<int,int>(r,c);

                // if already a citizen of some nation
                if (getCitizenship(tribe_leader) != -1) continue;

                // if already a member of some tribe
                if (getTribeMembership(tribe_leader) != -1) continue;

                // gather the tribe members
                Tribe dummy;
                Tribe newTribe = BFS(tribe_leader, dummy);
                
                // the group looks for a nearby tribe to settle down
                int nearbyT = searchNearbyTribe(newTribe);
                if (nearbyT == -1)  // if there's no tribe nearby
                {
                    if (newTribe.size() >= TRIBE_MIN_POPULATION) {
                        for (auto n : newTribe) 
                            map[n.first * N + n.second] = tribes.size() + nations.size();
                        tribes.push_back(newTribe);
                    }
                }
                else                // found nearby existing tribe, add this group of lives to the tribe found
                {   
                    tribes[nearbyT].insert(tribes[nearbyT].end(), newTribe.begin(), newTribe.end());
                    for (auto n : newTribe) 
                        map[n.first * N + n.second] = nearbyT + nations.size();
                }
            }
        }
    }

    void generate_new_nation(Tribe t)
    {
        if (t.size() < NATION_MIN_POPULATION) return;
        Nation newNation;

        newNation.color = available_colors.front();
        available_colors.erase(available_colors.begin());

        newNation.land = t;
        newNation.people = t;
        newNation.years_of_history = 1;
        newNation.technology_index = 2;
        int nation_index = nations.size();
        for (auto p: t) map[p.first * N + p.second] = nation_index;

        nations.push_back(newNation);
    }

    void expand_nations()
    {
        vector<vector<Member>> neighbors_list;
        for (int i = 0; i < nations.size(); i++) {
            // find new neighbors
            vector<Member> new_neighbors = BFS_for_nation(nations[i].people);
            neighbors_list.push_back(new_neighbors);
        }

        // check duplicates
        for (int i = 0; i < neighbors_list.size(); i++) {
            for (int j = i+1; j < neighbors_list.size(); j++) {
                for (int ii = 0; ii < neighbors_list[i].size(); ii++) {
                    for (int jj = 0; jj < neighbors_list[j].size(); jj++) {
                        Member a = neighbors_list[i][ii], b = neighbors_list[j][jj];
                        if (a.first == b.first && a.second == b.second) {
                            // detected duplicate
                            cout << "Found dup between nation " << i << " and " << j << endl;
                            int loser = (whose_closer(a, i, j) == i) ? j : i;
                            cout << "--> Loser is " << loser << endl;
                            int lost_member = loser == i ? ii : jj;
                            neighbors_list[loser][lost_member] = pair<int, int>(-1, -1);
                        }
                    }
                }
            }
        }
        auto dup_predicate = [](Member p){ return p.first == -1;};

        // delete duplicates
        for (int i = 0; i < neighbors_list.size(); i++) {
            neighbors_list[i].erase(
                remove_if(neighbors_list[i].begin(), neighbors_list[i].end(), dup_predicate),
                neighbors_list[i].end());
        }


        for (int i = 0; i < nations.size(); i++) {
            // nation index
            Member p = nations[i].people.front();
            int nation_index = map[p.first * N + p.second];

            nations[i].people.insert(nations[i].people.end(), neighbors_list[i].begin(), neighbors_list[i].end());

            // expand territory and mark new territory on map
            nations[i].land.insert(nations[i].land.end(), neighbors_list[i].begin(), neighbors_list[i].end());
            for (auto n: neighbors_list[i]) map[n.first * N + n.second] = nation_index;
        }
    }

    void init_nations()
    {
        nations = history.nations;
        for (int nat = 0; nat < nations.size(); nat++) {

            // update years of history and technology index
            nations[nat].years_of_history++;
            nations[nat].technology_index += nations[nat].years_of_history*2;

            // update census
            nations[nat].people.clear();
            for (auto point: nations[nat].land) {
                // if there's life in this address
                if (grid[point.first * N + point.second] > 0) 
                    nations[nat].people.push_back(point);
            }

            // Take its color off the list of publicly available colors
            for (int i = 0; i < available_colors.size(); i++) {
                if (nations[nat].color == available_colors[i]) 
                    available_colors.erase(available_colors.begin()+i);
            }
        }
        expand_nations();
    }

    void init_world()
    {
        map = history.world_map.matrix;

        init_nations();
        init_tribes_BFS_only();
        
        vector<Tribe> remainTribes;
        for (int i = 0; i < tribes.size(); i++)
        {
            if (tribes[i].size() >= NATION_MIN_POPULATION) {
                cout << "Tribe " << i << " upgrades to nation!!!" << endl; 
                generate_new_nation(tribes[i]);
                // tribes.erase(tribes.begin() + i);
            }
            else {
                remainTribes.push_back(tribes[i]);
                cout << "Tribe " << i << " remains a tribe :(" << endl; 
            }
        }
        tribes = remainTribes;

        //update tribe indices
        for (int i = 0; i < tribes.size(); i++) {
            for (auto p: tribes[i]) {
                map[p.first * N + p.second] = nations.size() + i;
            }
        }

        for (int i = 0; i < tribes.size(); i++) {
            cout << "tribe " << i;
            cout << ": people: " << tribes[i].size();
            Member p = tribes[i].front();
            int membership = map[p.first * N + p.second];
            cout << "; color of (" << p.first << ", " << p.second << ") of membership " << membership << " : ";
            cout << available_colors[(membership - nations.size()) % available_colors.size()] << "   "  << RESET << endl;
        }
        for (int i = 0; i < nations.size(); i++) {
            cout << "nation " << i;
            cout << ": people: " << nations[i].people.size();
            cout << "; land: " << nations[i].land.size();
            cout << "; color: " << nations[i].color << "   "  << RESET << endl;
        }

        cout << "Available colors: " ;
        for (int i = 0; i < available_colors.size(); i++) {
            cout << available_colors[i] << "   "  << RESET;
        }
        cout << endl;
    }


    vector<Member> BFS_for_nation(vector<Member> people) {
        // initialize queue
        vector<Member> queue = people;
        vector<Member> visited;
        vector<Member> new_neighbors;

        //find neighbors
        int radius = range/2;
        while (!queue.empty())
        {
            Member p = queue.front();
            if (!isInTribe(p, visited)) {
                visited.push_back(p);
                if (!isInTribe(p, new_neighbors) && !isInTribe(p, people)) new_neighbors.push_back(p);
                for (int r = p.first - radius; r <= p.first + radius; r++) {
                    for (int c = p.second - radius; c <= p.second + radius; c++) {
                        if (r == p.first && c == p.second) continue;                           // neighbor is p itself
                        bool withinBound = (0 <= r && r < M) && (0 <= c && c < N);
                        if (!withinBound) continue;                                            // neighbor is out of bound
                        if (grid[r*N+c] == 0) continue;                                        // neighbor is not alive

                        Member neighbor = pair<int,int>(r,c);
                        if (getCitizenship(neighbor) != -1) continue;                          // neighbor is already a citizen
                        if (isInTribe(neighbor, visited)) continue;                            // neighbor is already visited
                        if (isInTribe(neighbor, queue)) continue;                              // neighbor is already in queue
                        if (isInTribe(neighbor, people)) continue;                             // neighbor is already in this nation

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
                        if (getCitizenship(neighbor) != -1) continue;                          // neighbor is in other nation
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

    vector<string> available_colors = {BOLDYELLOW, BOLDGREEN, BOLDBLUE, BOLDRED, BOLDMAGENTA, BOLDCYAN, BOLDBLACK, BOLDWHITE};

    Map(int M, int N, int range, vector<int> grid, History history) : 
        M(M), N(N), range(range), grid(grid), history(history)
    {
        init_world();
    }

    void war()
    {
        if (nations.size() == 4) {
            for (auto n: nations) 
                if (n.land.size() < 150) return;
            int nation1 = rand() % 4;
            int nation2 = rand() % 4;
            while (nation2 == nation1) nation2 = rand() % 4;
            int ifwar = rand() % 10;
            ifwar = (ifwar == 1) ? 1 : 0;
            if (ifwar) {
                for (int i = 0; i < 3; i++) {
                    print_map(ifwar, nation1, nation2);
                    usleep(200000);
                    print_map();
                    usleep(200000);
                }
            }
        }
    }

    /* 
     * print map with nations and tribes 
     * 0 = barren; 1 = barbarian; 2+ = nation/tribe index 
     */
    void print_map(int war = 0, int nation1 = -1, int nation2 = -1)
    {
        assert(available_colors.size() + nations.size() == 8);
        for (int r = 0; r < M; r++) 
        { 
            for (int c = 0; c < N; c++) 
            {   
                int membership = map[r*N+c];
                int life = grid[r*N+c];

                // determine color
                if (membership != -1) {
                    if (membership < nations.size()) {
                        if (war == 1 && (membership == nation1 || membership == nation2))
                            cout << BOLDBLACK;
                        else cout << nations[map[r*N+c]].color;
                    }
                    else  
                        cout << available_colors[(membership - nations.size()) % available_colors.size()];
                }

                // determine life or death
                if (life) cout << "* " << RESET; 
                else      cout << ". " << RESET; 
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

        // erase tribes
        for (auto t: tribes) {
            for (auto p: t) {
                history.world_map.matrix[p.first*N+p.second] = -1;
            }
        }
        return history;
    }

};
