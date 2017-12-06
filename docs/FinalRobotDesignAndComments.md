# __*Final Comments on Robot*__

### Design

[INSERT PICTURE HERE]

#### The Physical Design
We decided to stick the orginal design from the beginning of the class and simply build from there. we had an ordinary chassis, midsize wheels with rubber grip for better traction, a section underneath the base for the power bank, a ball bearing for stability, and we added a second layer to provide us with space to wire our circuit. We passed most of our wires through different openings in our top level chassis. We tried to keep the wires relatively organized and typically kept all ground and power wires, black and red, respectively. This helped keep the area around the actual Arduino Uno less cluttered. On the top of our robot, we also included two 9 Volt batteries responsible for powering the rails of our robot, so that we could have a negative reference for certain circuits we included for treasure detection and possible filtering for the microphone. Our wall and treasure sensors were placed on the front, left, and right sides of the robot. Lastly, we used 3 line sensors, two to keep our robot on the lines and one slightly displaced to notify us of intersections. We also created an actual base station by velcroing a breadboard, our second Arduino, and our FPGA for ease of use in transporting the base station and keeping everything together.

(CAN SOMEONE DISCUSS HOW THE MUX WORKED AND WHAT WAS CONNECTED TO IT)

#### Algorithms and Display
We used a depth-first search to solve the maze, using the priority order of forward-left-right. At each intersection, the robot would transmit via the radio information about the surrounding walls, treasures, and the robot's location at that node. At the base station, this information was parsed and put together in our display. A green dot means that the node has been visited, a red line is a wall, and the treasures were designated at the nodes by yellow circles with the corresponding number in kHz for the frequencies, 7, 12 and 17 (in the colors red, green and blue respectively).

#### Potential Flaws in Design
Nearly all of the connections and circuits were placed on the very top of the robot, which meant that the wires had to go all the way from near the bottom where the sensors and Arduino Uno were, creating a lot of confusion when it came to debugging. Since we used a circuit board instead of a PCB, we were able to edit our circuits as often as we pleased, but that also meant wires came loose somewhat often and significant time was lost to tracing back these kinds of problems.

### Simulation vs. Experimental Results
It was hard to test in different environments because background sound and lightalways affect thresholds whereas in simulations those things can be held constant. Our robot and seemed to be working fine during all testing leading up to the final competition.

### The Final Competition



[Here's the video of our robot during the final competition](https://www.youtube.com/watch?v=oZpQe9s_qdU)

### Conclusions
Our robot was built without having to purchase any parts not already found in lab, so we used $0 of our $100 budget.

#### Suggestions for Future Improvements
* Solder working circuit, so there will be no chances of wires popping out. This can save a lot of time when testing.
* Use different layered boards so that wires wouldn't be blocking the Arduino when we needed to access it.
* Larger wheels to improve speed.




https://cei-lab.github.io/ece3400/Grading/Design_score.html

