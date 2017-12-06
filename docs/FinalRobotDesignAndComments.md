# __*Final Comments on Robot*__

### Design

![alt text](Finalpics/IMG_20171204_115202_ds.jpg)
![alt text](Finalpics/IMG_20171204_115205_ds.jpg)

#### The Physical Design
We decided to stick the original design from the beginning of the class and simply build from there. We had an ordinary chassis, midsize wheels with rubber grip for better traction, a section underneath the base for the power bank, a ball bearing for stability, and we added a second layer to provide us with space to wire our circuit. We passed most of our wires through different openings in our top level chassis. We tried to keep the wires relatively organized and typically kept all ground and power wires, black and red, respectively. This helped keep the area around the actual Arduino Uno less cluttered. On the top of our robot, we also included two 9V batteries responsible for powering the rails of our robot, so that we could have a negative reference for certain circuits we included for treasure detection and possible filtering for the microphone. Our wall and treasure sensors were placed on the front, left, and right sides of the robot. Lastly, we used three line sensors, two to keep our robot on the lines and one slightly displaced to notify us of intersections. We ran into some interesting behavior with our treasure detectors, and realized our band-pass filter picked up the IR signals from our wall detectors, so we included a high side transistor on our IR wall sensors VCC= to shut off the wall sensors when the treasure detectors were active.
We also created an actual base station by velcroing a breadboard, our second Arduino, and our FPGA for ease of use in transporting the base station and keeping everything together.

(CAN SOMEONE DISCUSS HOW THE MUX WORKED AND WHAT WAS CONNECTED TO IT)

#### Algorithms and Display
We used a depth-first search to solve the maze, using the priority order of forward-left-right. At each intersection, the robot would transmit via the radio information about the surrounding walls, treasures, and the robot's location at that node. At the base station, this information was parsed and put together in our display. A green dot means that the node has been visited, a red line is a wall, and the treasures were designated at the nodes by yellow circles with the corresponding number in kHz for the frequencies, 7, 12 and 17 (in the colors red, green and blue respectively).


#### Potential Flaws in Design
Nearly all of the connections and circuits were placed on the very top of the robot, which meant that the wires had to go all the way from near the bottom where the sensors and Arduino Uno were, creating a lot of confusion when it came to debugging. Since we used a circuit board instead of a PCB, we were able to edit our circuits as often as we pleased, but that also meant wires came loose somewhat often and significant time was lost to tracing back these kinds of problems.

### The Final Competition
Unfortunately for us, our robot did not work the way we planned during the competition. We noticed weird behavior when testing in the mazes in the Duffield atrium, which lead to some rapid debugging and quick fixes, and afterward noticing that a wire had come loose, so the changes we made served to mess up how the robot made its detections completely.

When it actually came to our first run, our robot drove two squares, then the power bank decided to die and we were unable to try again until the next round. The video below is from our second attempt (after charging the power bank after the first attempt and some quick fixes), where you see a false detection from our front wall sensor. Assuming there would have been a wall there, the depth-first search did work properly, and our display signaled done and our tone did play. We tried running the robot again, and it continued to keep having false wall detections, and again we were unable to complete the maze. After our final imperfect round, we realized one of the two ground wires from the Arduino to the breadboard's GND rail had come loose, causing the breadboards GND to be at a slightly high voltage than our Arduino's GND. Unfortunately, this was enough to cause `analogRead()` on the front wall sensor to be slightly higher than we were expecting it to be, and as a result our threshold detector kept falsely detecting walls.  

The treasure detectors were perhaps a bit too sensitive and had many false detections, but the one genuine treasure that it did pass it did pick up, as reflected in our display. Similar to why the treasure detectors, the microphone circuit which used nearly the same code, did not pick up the start tone.


[Here's the video of our robot during the final competition](https://www.youtube.com/watch?v=oZpQe9s_qdU)


While we were all disappointed in our performance in the competition, we did take a video from the night before, [showing our robot and display working correctly](https://www.youtube.com/edit?o=U&video_id=NwgnMxAbo9s).



#### Suggestions for Future Improvements
* Solder working circuit, so there will be no chances of wires popping out. This can save a lot of time when testing.
* Use different layered boards so that wires wouldn't be blocking the Arduino when we needed to access it.
* Larger wheels to improve speed.

#### Cost
The total cost of our robot was $63, coming in at $37 below the budget. The cost came from our line sensors, distance sensors, microphone (with gain), and servos. All other components of the robot were spare parts found in the lab (e.g. Arduino Unos, FPGA, wires, resistors, capacitors, etc.) at no additional cost to us.

### Conclusions
It was hard to test in different environments because background sound and light always affect thresholds whereas in simulations those things can be held constant. Our robot and seemed to be working fine during all testing leading up to the final competition. Loose wires were the main cause for the poor showing in the competition, which was compounded with other small errors.

In the end, it was a fun, albeit stressful semester which gave all of us experience actually implementing many different ECE concepts we have learned in other classes, as well as experience working in a team, and many opportunities to make mistakes and learn from them. We all gained an appreciation for the development process for complicated intelligent physical systems, from using effective communication to complete the the many smaller goals needed in order to get to our final robot.


https://cei-lab.github.io/ece3400/Grading/Design_score.html
