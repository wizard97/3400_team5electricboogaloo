# Milestone 1

### Objective
The objective of this milestone was to add line-following and turn functionality to our robot. More specifically, this entailed correctly implementing two "inner" line sensors (for line-following functionality) and two "outer" line sensors (for detecting intersections and turn functionality). Ultimately, these two core functionalities were combined, allowing our robot to move in a figure eight path.

### Work Distribution
The entire team worked on this milestone. While the acoustic team for Lab 2 -- Adam, Kelsey, and David -- finished up Lab 2, the optical team -- Rajiv, Aaron, and Christina -- was responsible for the documentation of this milestone.

### Lab Documentation

#### Materials
* Line sensors
* 3-pin headers
* Wires
* Breadboard

#### Line-Following
We first connected all four line sensors to the Uno by soldering headers to each of the sensors and then connecting the headers to analog pins on the board with wires. Once the line sensors were connected, we tested them (i.e. "rolled" over black tape on white paper several times) in order to obtain approximate threshold values above which our robot would be told to register the color black and below which our robot would be told to register the color white. Only the two "inner" line sensors are used for line-following, as their placements on the robot correspond with being "on" the black line, given that the robot is centered on the line. Thus, if the left "inner" line sensor detects white, then our robot steers back towards the right; if, instead, the right "inner" line sensor detects black, then our robot steers back towards the left. See our code below for more clarification.

```c++
if(fleft_ir < THRESHOLD) { // left sensor on white
  turnRight(10);
} else if(fright_ir < THRESHOLD) { // right sensor on white
  turnLeft(10);
} else {
  forward(10);
}
```

See [here](https://www.youtube.com/watch?v=ddbK3lsctK8) for a video demonstration of our robot's line-following functionality!

#### Figure Eight Path
The two "outer" line sensors are used for perpendicular line detection (i.e. for detecting an intersection). When the "outer" line sensors detect black, the robot registers that it has traveled one "block". Additionally, it can be commanded to turn either clockwise or counter-clockwise by speeding up the left servo in the forward direction and the right servo backwards, or the right servo in the forward direction and the left servo backwards, respectively.

```c++
switch(wideir_state) {
  case(SWIR_NO_LINE): default:
  if(oleft_ir > THRESHOLD && oright_ir > THRESHOLD) { // both outer sensors on black
    wideir_state = SWIR_ON_LINE;
  }
  break;

  case(SWIR_ON_LINE):
  if (oleft_ir < THRESHOLD && oright_ir < THRESHOLD) { // both outer sensors back on white
    wideir_state = SWIR_PASSED_LINE;
    line_count++;
  }
  break;

  case(SWIR_PASSED_LINE):
  wideir_state = SWIR_NO_LINE;
  break;
}

// Figure eight
if(wideir_state == SWIR_PASSED_LINE && line_count%4) {
  if(line_count%8 <= 3 ) {
    turnRightIP();
  } else if(line_count%8 > 4) {
    turnLeftIP();
  }
}
```

As shown by our code above, our algorithm for the figure eight specifically first assumes that our robot does NOT start at an intersection, and then uses a counter (mod 8) to determine whether to turn clockwise, keep straight, or turn counter-clockwise. We also implemented a state machine with three states: (1) the state in which the "outer" line sensors are not detecting a line (i.e. are on white), (2) the state in which the "outer" line sensors are detecting a line (i.e. are on black), and (3) the state in which the robot just passed the intersection.

See [here](https://www.youtube.com/watch?v=XTS3uJ2DYwY) for a video demonstration of our robot following a figure eight path!
