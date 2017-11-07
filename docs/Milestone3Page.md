# __*Milestone 3*__

### Objective
The objective of this milestone was to implement a path-traversing algorithm to facilitate maze exploration. This involved first running our algorithm in a simulation and then running our algorithm on our robot. In both the simulation and the real-life simulation, there is an indicator for when maze exploration is complete.

### Work Distribution
The entire team developed the algorithm together. Adam and David worked on the implementing and debugging the simulation; Christina, Aaron, Kelsey, and Rajiv worked on implementing and debugging the real-life application of the algorithm.

### Lab Documentation

#### Algorithm
The team decided to implement a random-search algorithm to facilitate maze exploration. At each "node", the algorithm uses data about on its left, front, and right sides (i.e. whether or not a wall is present) to determine the nodes that are "available" to visit next. Our algorithm then randomly chooses one of the available nodes to visit.

```c++
```

##### Finish Detection
Detecting that the robot should stop was the most complicated part of the algorithm. Each time a new node was visited, the visited field would be set and each edge_t would be updated to reflect the border, if a wall was detected the associated isWall would be set to true. To determine that every possible node was visited, a simple DFS algorithm would run and check to make sure no nodes had the visited field unset. Our function that checks this was recursive, and would stop recursing once isWall was set or node was NULL. We intend to eventually make this function non-recursive to make sure we don't blow out the stack on the microcontroller. If the recursive DFS function could not find a node that was not visited it would return true. To make our robot stop where we started, we had an x-offset-counter and y-offset-counter that was initilized to zero. Each black line the robot passed would increment or decrement the appropriate counter. Once our DFS algorithm detected that we visited all the nodes, and that our offset counters were both 0 (the robot is in the starting position), the robot would stop and light up the done LED.

```c++
typedef struct Edge
{
  bool isWall;
  node_t *node;
} edge_t;

typedef struct Node
{
  bool visited;
  edge_t left;
  edge_t right;
  edge_t up;
  edge_t down;
} node_t;
```


#### Simulation
For our simulation, we implemented a very basic Depth-first search to demonstrate how our robot would explore the maze in real time. Because our maze is very small, the possibilities for DFS paths are quite limited, and most paths of exploration are quite predictable merely by inspection. Such is mostly the case for our own demonstration, however we attempt to create a maze with enough variability as to demonstrate the nature of our algorithm.

We began by designing the maze. For this, we first looked on the main website for a sample maze that would fit the depth first search algorithm. We used something very similar to Team Alpha’s maze, and in fact modeled our maze in a similar manner, using various of the paint, line, imagesc, caxis, etc. and various other native graphical functions for drawing purposes. Using imagesc, we were able to very simply represent the current position of our robot in the process of exploration. While the graphical interface will look very different in our real-world implementation using the fpga, this representation serves to demonstrate the algorithm in a real-time environment.

For the actual algorithm, we found an implementation of the DFS queue and stack in matlab online, and tried to incorporate it into our own design, in a fashion similar to Team Alpha’s. There were various issues we found with integrating the stack and queue found online with our own graphical representation of the maze, and so we made significant modifications to essentially strip the online implementation down to make it more manageable given the small scale of our maze.

A video of our DFS simulation is shown here:

ADD VIDEO HERE


#### Real Life
Despite the bugs in our algorithm, our robot successfully completed maze exploration; see [here]() for the video demonstration!
