# __*Milestone 4*__

### Objective
The objective of this milestone was to have a fully running robot that can traverse a maze and display it simultaneously. It must also play a done signal and show finished screen upon completion of the maze. 

### Work Distribution
The entire team worked on the robot together. 

### Mapping The Maze
* First, we fixed the errors in our DFS algorithm from milestone 3. We were able to stop the robot from traversing back to the beginning to finish. 
* Next, we had to fuse our current code with the radio transmission code from lab 4. The ping out code would be fused with the algorithm code and the receiving code with our graphics display. First, we made a struct with parameters for the x position of the robot, the y position, the orientation (positive x, negative y, etc...), the surrounding objects (walls, treasures, clear paths), and whether the algorithm is done or not. We then fill in the data of a struct object using our current algorithm code and this code is sent to the base station at every node. Next, on the receiving end, we create the same struct in a different file and this is used so we can read the send data and then update the display by drawing a dot for the current coordinate of the robot and also drawing the walls or treasures. 
(add in code for this paragraph, perhaps the struct)

### Signaling Done
To signal done, we needed to both play a sound upon completion of the maze and display a done screen. This was a fairly simple implementation. Since we're already sending a done parameter via radio, we simply need to check this parameter and then run the two different finishing codes accoringly in our code. So we added our tone from lab 3 and added code that would write done on our screen in large text and both are enabled when the proper done signal is received.
(more code)

### Treasure detection
For this milestone, we had to revamp our treasure detection slightly. For one, due to all of our analog pin needs, we created a mux. This mux would allow us tohave extra connections. We ended up connection our wall sensors to it as this seemed to be the easiest option. We also used the same circuit from our original treasure detection in lab 2 and used fft so that we could read each different frequency of treasure. The problem, however, is that the treasure detection cannot work while the wall sensors are on, so we attempted to create a transistor switch so that we could toggle power to the wall detectors as needed when we need to use the treasure detection. 

