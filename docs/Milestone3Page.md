# __*Milestone 3*__

### Objective
The objective of this milestone was to implement a path-traversing algorithm to facilitate maze exploration. This involved first running our algorithm in a simulation and then running our algorithm on our robot. In both the simulation and the real-life simulation, there is an indicator for when maze exploration is complete.

### Work Distribution
The entire team developed the algorithm together. Adam and David worked on the implementing and debugging the simulation; Christina, Aaron, Kelsey, and Rajiv worked on implementing and debugging the real-life application of the algorithm.

### Lab Documentation

#### Algorithm
The team decided to implement a random-search algorithm to facilitate maze exploration. At each "node", the algorithm uses data about on its left, front, and right sides (i.e. whether or not a wall is present) to determine the nodes that are "available" to visit next. Our algorithm then randomly chooses one of the available nodes

```c++
```

#### Simulation

#### Real Life
Despite the bugs in our algorithm, our robot successfully completed maze exploration; see [here]() for the video demonstration!
