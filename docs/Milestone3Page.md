# __*Milestone 3*__

### Objective
The objective of this milestone was to implement a path-traversing algorithm to facilitate maze exploration. This involved first running our algorithm in a simulation and then running our algorithm on our robot. In both the simulation and the real-life simulation, there is an indicator for when maze exploration is complete.

### Work Distribution
The entire team developed the algorithm together. Adam and David worked on the implementing and debugging the simulation; Christina, Aaron, Kelsey, and Rajiv worked on implementing and debugging the real-life application of the algorithm.

### Lab Documentation

#### Algorithm
The team decided to implement a random-search algorithm to facilitate maze exploration. At each "node", the algorithm uses data about on its left, front, and right sides (i.e. whether or not a wall is present) to determine the nodes that are "available" to visit next. Our algorithm then randomly chooses one of the available nodes to visit. See below for an explanatory snippet of our code.

```c++
```

As for determining when we are finished with maze exploration, //TYPE MORE HERE!!

```c++
```

#### Simulation

#### Real Life
To run our algorithm on our robot, we integrated our line detection and wall-sensing code from [Milestone 1](Milestone1Page.md) and [Milestone 2](Milestone2Page.md), respectively, for moving through the maze.

Despite the bugs in our algorithm, our robot successfully completed maze exploration; see [here](https://www.youtube.com/watch?v=RCDYiQO79UA&index=1&list=PLpzyLEaV2FZwIKaHse5H3hJURWajDci3Z) for the video demonstration!
