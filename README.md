## GAME OF CIVILIZATION

## Authors
Yating Han (yatingh)  
Yuying Qian (yuyingq)

## Links
<a href="https://github.com/yatinghan/game_of_civilization"> Project Repo </a>  
<a href="https://yatinghan.github.io/game_of_civilization"> Project Website </a>  

## Table of contents
[Running the Game](#running_the_game)  
[Project Proposal](#project_proposal)  
[Work Breakdown](#work_breakdown)  
[Contacts](#contacts)  

## Running the Game <a name="running_the_game"></a>
No code yet :‘(

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

## Work Breakdown <a name="work_breakdown"></a>
Yating: 50%  
Eryn: 50% 


## Contacts <a name="contacts"></a>

<a href="https://github.com/yatinghan" >Yating Han</a> (yatingh at andrew)   
<a href="https://github.com/yatinghan"> Yuying "Eryn" Qian</a> (yuyingq at andrew)
