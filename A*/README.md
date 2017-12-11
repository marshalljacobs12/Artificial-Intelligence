# Navigation in Partially Known Environments

### Description
This project implements two variants of A\*, Repeated Forward A\* and Adaptive A\*, to navigate a partially known grid.

### Running the Application
To run the solver, navigate to the fall16-proj3-source folder and simply use
```
make
```
to compile the code. Then, use
```
./GridPathPlanner
```
to execute the program.

### Comparison of A\* Methods:

Compare Repeated Forward A* and Adaptive A* with respect to the number of states they expand. You should compare the number of expansions that both algorithms perform during their first search, the number of expansions during their second search, and the average number of expansions over all searches until the target is reached. Explain your observations in detail, that is, explain what you observed and provide a reason for your observations.

Repeated Forward A\*
* Search 1: 155 expansions
* Search 2: 155 expansions
* Total Expansions: 13070
* Average Expansions: 13070 / 140 = 93.36 expansions / search

Adaptive A\*
* Search 1: 155 expansions
* Search 2: 154 expansions
* Total Expansions: 8219
* Average Expansions: 8219 / 140 = 58.71 expansions / search

The number of expansions I observed for A\* and Adaptive A\* are consistent with the theoretical basis for using Adaptive A\* search. Because the Adaptive A\* h-values are at least as large as the A\* h-values (Manhattan Distance heuristic), Adaptive A\* should never expand more states than A\*. During Search 1, both A\* and Adaptive A\* have the same h-values, the Manhattan Distance h-values which the problem is initialized with. Therefore, it follows that A\* and Adaptive A\* have the same number of expansions during Search 1. During Search 2, the nodes expanded in Search 1 have updated h-values in Adaptive A\* (the h-values in repeated A\* are the original Manhattan Distance values for every search). As mentioned, these updated h-values dominate the Manhattan Distance h-values, so A\* should have at least as many expansions as Adaptive A\* in Search 2 and beyond. The benefit of these updated Adaptive A\* h-values is small in Search 2 (154 expansions vs 155 expansions), but as the Adaptive A\* progresses toward the goal, the savings in number of expansions increase. Overall, the savings in number of expansions from Adaptive A\* (58.71 expansions / search) compared to Repeated A\* (93.36 expansions / search) are significant, so Adaptive A\* is a useful optimization.