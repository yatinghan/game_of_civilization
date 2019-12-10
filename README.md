## GAME OF CIVILIZATION

## Authors
Yating Han (yatingh)  
Yuying Qian (yuyingq)

## Links

<a href="https://github.com/yatinghan/game_of_civilization"> Project Repo </a>  
<a href="https://yatinghan.github.io/game_of_civilization"> Project Website </a>  

## Table of contents
[Running the Game](#running_the_game)  
[Final Report](#final_report)  
[Project Checkpoint](#project_checkpoint)  
[Project Proposal](#project_proposal)  
[Work Breakdown](#work_breakdown)  
[Contacts](#contacts)  
____

## Running the Game <a name="running_the_game"></a>
Game of Life: under GoL, run make seq or make cuda  
Game of Civilization: under GoC, run make baseline or make cuda  



## Project Report <a name="final_report"></a>
<a href="https://github.com/yatinghan/game_of_civilization/blob/master/Project%20Report%20-%20Google%20Docs.pdf"> Project Report </a>  
____


## Project Checkpoint <a name="project_checkpoint"></a>

### Project Status
We’ve successfully implemented the original game of life. It does not have a visualization yet. However, we can see it working from terminal output. However, we discovered some major design flaws in the game. In the proposal, we described three types of “civilizations”: village, city, and nation. As we discussed the implementation details, we realized that area division of these entities are exactly the same in terms of algorithm design / parallelization: we could just traverse through each “square” and determine if it meets the requirements we defined. A major flaw in this design is that every entity could be characterized by multiple overlapping squares, depending on where the center is. In addition, we proposed that all nations should be squares which is not the case in real world. Therefore, we want to switch to a simpler rule (with just “tribes” and “nations”) but use a more precise (and interesting) algorithm for characterization. 

Here are the revised rules:
<details><summary>Original Rules </summary>
  
- Any live cell with fewer than two live neighbours dies, as if by underpopulation.  
- Any live cell with two or three live neighbours lives on to the next generation.  
- Any live cell with more than three live neighbours dies, as if by overpopulation.  
- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.  

</details>

<details><summary>Map</summary>
  
- Any living population belongs to a tribe. Nearby lives belongs to the same tribe. (detailed rule determined by the search algorithm)  
- If tribes are in close proximity, they will unite with each other and form a nation.

</details>

<details><summary>Technology & War</summary>
  
- Only tribes above a basic power level may declare war.   
- Truce condition: Entities engaged in war in a certain round will not go to war again in the next 5 rounds.  

</details>

The first phase is still updating the pixels, which follows the original game rules. It will be done with a relatively simple CUDA function because each pixel only depends on itself and its neighbor. The second phase will be doing some graph searching or clustering on the 2D array and determining the boundaries of the cities and nations. We are still planning to use CUDA, subjected to change as discussed in the issue section. It is more challenging than the algorithm we described in the proposal, as graphs are inherently recursive data structure and is hard to parallelize using CUDA. We will use BFS which offers more opportunities for parallelism. Finally, we will pairwise-compare the civilizations and determine if they should go to war, then update the pixels once again.

### Evaluation
Because we revisited the rules, we are about one week behind schedule. However, we believe that we will still get all the deliverables done because we will both stay on campus during Thanksgiving break. As we revisited the algorithm, we will probably have more interesting nice-to-haves such as implementing different graph searching algorithm or clustering. 

We plan to achieve the following:
- Build the GUI that takes in an initial population configuration and runs the evolution simulation.  
- Implement all the rules described in Section Background.  
- Parallelize the algorithm with Cuda and OpenMP.  
Nice-to-haves:
- Explore more frameworks on graphs.  
- Implement a version with clustering.  

### Issues
We noticed several loopholes in our design of the game rules, like the characterization of entity territory(mentioned in the first section), the expansion rule of a nation (as of now, once the first nation is established, it will soon devour all the nearby entities because the war condition is too easy to be triggered), lack of truce condition (if two entities go to war in a certain round, they will continue fighting in the subsequent rounds until one of them dies or downgrades), etc etc. We realize game design is not as simple as we thought it is :)

We are trying to evaluate which programming model works the best for graph computation. When traversing the map to compute area division of the cities, each 10x10 block is treated as a vertex on the graph and the close ones are grouped into the same city(vertex). To find nations, every city is treated as a vertex and vertices(cities) that are close to each other are grouped together to form a cluster(nation). Although we used CUDA to implement the basic rules of life, where each member of the population is evaluated using the exact same logic, it might not be the best choice for graph computation. From what we learned from assignment 3, it seems OpenMP would work better for algorithms involving recursion. However, the program does not work quite right. We’re still debugging the issue. Our next step is either improving our existing algorithms or exploring new frameworks like GraphLab or trying new algorithms like K-means clustering (which we now think might be a more reasonable choice). Experiments with different kinds of implementations could be pretty time consuming. 

### Updated Schedule 
<table>
  <tbody>
    <tr>
      <th>When</th>
      <th>What</th>
      <th>How</th>
      <th>Who</th>
    </tr>
    <tr>
      <th>November Week 3 A</th>
      <th>The parallel implementation</th>
      <th>Use CUDA to parallelize basic rules of life</th>
      <th>Both</th>
    </tr>
    <tr>
      <th>November Week 3 B</th>
      <th>BFSn</th>
      <th>Parallelize BFS algorithm in area division </th>
      <th>Both</th>
    </tr>
    <tr>
      <th>November Week 4 A</th>
      <th>K-means Clustering</th>
      <th>Parallelize K-means clustering in area division</th>
      <th>Both</th>
    </tr>
    <th>November Week 4 B</th>
      <th>Parameter tuning</th>
      <th>Optimize the performance</th>
      <th>Both</th>
    </tr>
    <tr>
      <th>December Week 1</th>
      <th>Final project report (10 pages)</th>
      <th>Final project report submission by Friday, Dec 6th.</th>
      <th>Both</th>
    </tr>
  </tbody>
</table>

## Project Proposal <a name="project_proposal"></a>

### Project Summary
We are going to implement an enhanced version of Conway’s Game of Life, i.e. Game of Civilization, on GPU, to simulate the evolution of lives, villages, cities and nations.  

### Background
In the original Game of Life, the status of every cell in the world follows a simple set of rules:
- Any live cell with fewer than two live neighbours dies, as if by underpopulation.
- Any live cell with two or three live neighbours lives on to the next generation.
- Any live cell with more than three live neighbours dies, as if by overpopulation.
- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

![Orignal Game](images/original.png)

To make the evolution more interesting, we will add a new set of rules to simulate the development of villages, cities and nations:  
<details><summary>Map</summary>

- Any 20x20 block with more than 20 living cells would automatically upgrade to a village.
- Any 50x50 block with more than 50 cells would upgrade to a city. 
- Any 1000x1000 block with more than 20 cities would upgrade to a nation. 
- Once an entity is upgraded to a higher level, it will never downgrade even when the population falls below the criteria of its current level, unless population dies down to zero or it’s conquered by an entity of lower level. (See the War section)  
</details>

<details><summary>Technology</summary>

Let the history of a village/city/nation be the number of years it has existed, n.
- The technology index of a village or city = n/4
- The initial technology index of a nation (when n=0) follows the city with highest technology index. The index would increment by 2 every subsequent year.   
</details>

<details><summary>War</summary>

- Power of any entity = technology index * population
- Entity with higher power wins the war and conquers the opponent entity. 
- Villages will not engage in wars. 
- War casualty = (opponent power / entity power) * ( ¼ * population) This equation computes how many cells would die in a war. Cells are killed at random. 
- Any two cities D1 distance apart or closer would go to war. The map remains unchanged. 
- Any nation and city D2 distance apart or closer would go to war. 
    * If the nation conquers the city, the city will be colonized. The resulting technology index will follow whoever is higher. 
    * If the city wins, the nation collapses: the map will recompute the area division. New cities and villages will inherit the nation’s technology and operate independently.
- Any two nations D3 distance apart or closer would go to war. The winner will colonize the loser if the power ratio (winner power/ loser power) is greater or equal to 2. Otherwise, the map remains unchanged. 
</details>

### Approach 
By the end of the semester, we want to parallelize the simulation by breaking it down to the following:
- Compute whether the cell is dead or alive for all cells.
- Find all entities on the map. 
- Track technology advancement for all existing entities. 
- Search enemies close to the border for all entities and update the entities according to the war results. 
These segments of computation could benefit from parallelism because the homogenous computation is applied many times on a large number of inputs.

### The Challenge
- Load balancing: Entities are not evenly distributed on the map, thus load balancing is important to achieving optimal performance. 
- Minimize communication between processors: each cell needs the information of all neighbors in order to determine its own status in the next epoch. 
- Efficient search of entities: there are multiple potential approaches of doing this, like QuadTree, convolution, or a naive search by cities. We need to analyze the complexity, communication costs, locality and memory access characteristics of the algorithms before we can determine which one might achieve the highest performance. We may use OpenMP instead of CUDA for this part of the computation, depending on our conclusion of the algorithm analysis. 

### Resources
We are planning to run the game on GPU. Depending on the scope of the project, we might also implement a CPU version and compare the results. The code will be written from scratch. 

### Goals and Deliverables
#### Plan to achieve:
- Build the GUI that takes in an initial population configuration and runs the evolution simulation. 
- Implement all the rules described in Section Background.
- Implement a sequential version and a parallel version respectively to examine the speedup. 
- Tune the parameters to optimize the performance.
#### Hope to achieve:
- Implement the game in ISPC on CPU and compare the performance with the CUDA version.
- We can add in more rules (e.g. rules of trade, rules of natural disasters, rules of diseases) to make the evolution more interesting. 
- Visualize the evolution progress with real time heat diagram of population distribution in the world.
#### Demo:
- A trial run of the game.

### Platform Choice
#### Computer:
We choose to run the game on GPU because a set of homogenous computation needs to be done for each cell and  it’s easy to map the cells in the world map to the threads on GPU. 

#### Language:
Kernel will be implemented in CUDA so that it can run on GPU, while the GUI will be implemented in Python to make our life easier. 


### Schedule
<table>
  <tbody>
    <tr>
      <th>When</th>
      <th>What</th>
      <th>How</th>
      <th>Who</th>
    </tr>
    <tr>
      <th>November Week 1</th>
      <th>The original Game of Life</th>
      <th>The game can take in an initial population configuration and simulate the evolution automatically which only follows the 4 fundamental rules of life.</th>
    </tr>
      <th>November Week 2</th>
      <th>Rules of War and Technology</th>
      <th>The game is able to identify (1) The villages, cities and nations on the map and mark the territories with different colors; (2) Which of the entities would declare war on each other and compute the population change due to the conflicts.</th>
    </tr>
      <th>Checkpoint: Nov 18</th>
      <th>Checkpoint Writeup</th>
      <th>Update the project web page and submit the writeup to Gradescope</th>
    </tr>
    <tr>
      <th>November Week 3</th>
      <th>The parallel implementation</th>
      <th>Implement the CUDA version of the game and examine the speedup. </th>
    </tr>
      <th>November Week 4</th>
      <th>Parameter tuning</th>
      <th>Optimize the performance</th>
    </tr>
      <th>December Week 1</th>
      <th>Final project report (10 pages)</th>
      <th>Final project report submission by Monday, Dec 9th.</th>
    </tr>
  </tbody>
</table>


## Work Breakdown <a name="work_breakdown"></a>
Yating: 50%  
Eryn: 50% 

____

## Contacts <a name="contacts"></a>

<a href="https://github.com/yatinghan" >Yating Han</a> (yatingh at andrew)   
<a href="https://github.com/yatinghan"> Yuying "Eryn" Qian</a> (yuyingq at andrew)
