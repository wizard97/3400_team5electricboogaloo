# __*Lab 4*__

### Objective
The objective of this lab was to first implement radio communication between two Arduinos and then integrate this functionality with the VGA component from Lab 3 to draw a complete maze on a VGA monitor and update the maze by transmitting information from one Arduino "radio" to the other.

### Work Distribution
As in the previous labs, we split into two sub-teams for this lab; Kelsey, Rajiv, and Christina worked on the radio component, and Aaron, David, and Adam worked on the FPGA component.

### Helpful Links
* [RF24 Class Reference](http://maniacbug.github.io/RF24/classRF24.html)

### Lab Documentation

#### Materials
* 2 Nordic nRF24L01+ transceivers
* 2 Arduino Unos
* 2 radio breakout boards
* FPGA
* 1 VGA cable
* 1 VGA connector
* 1 VGA switch
* Resistors

#### Radio Team

##### Setup

##### Implementation
[Here's a demonstration of the communication between the two Arduinos!](https://www.youtube.com/watch?v=0cg3QgOLpR0)

#### FPGA Team

##### Setup

##### Implementation
In [this](https://www.youtube.com/watch?v=aIYxe3nmHjY) video, you can see a demonstration of how we transmitted randomly generated coordinates from the transmitter radio to the receiver radio and then used this data to update our maze grid.
