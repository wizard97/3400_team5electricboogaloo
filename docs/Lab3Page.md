# __*Lab 3*__

### Objective
The objective of this lab was to

### Work Distribution
We split our lab group into two, with Adam, David and Aaron working on the VGA display portion and Kelsey, Rajiv and Christina working on the acoustic portion.
### Helpful Links

### Lab Documentation

#### Materials
* Arduino Uno
* FPGA DEO_nano
* Breadboard
* Resistors
* 4 Push Buttons
* VGA Screen
* VGA Connector
* 1 VGA Switch



#### Graphics Team
Setup
We began the assignment by examining the schematic for the FPGA so that we could find the GPIO pins we need to send the data to the screen. We then created a voltage divider between the Arduino and the FPGA because the FPGA operates on 3.3 V but the UNO outputs 5 V so we used a 1k ohm resistor and a 1.8k ohm resistor to drop the voltage from 5 V to 3.3 V. 

To send information to the FPGA we intially began implementing a parallel bus to send the data, but we realized this was not an ideal solution because it required too many pins, requiring us to individually toggle the pins to update the values and it also required us to have a valid bit. We decided to use a SPI driver to send the data. We found a template for a SPI online and modified it to accept three byte payloads with the first byte being the x-coordinate, the second being the y-coordinate, and the thrid being the color. The SPI gave us a significant performace increase over our parallel bus because we could use a 4 MHz clock to send information over it. The UNO has built in SPI hardware and operates on a 4 MHz clock so this gave us optimal speed while also minimizing the amount of wires we needed.

We then mapped a 120 by 120 grid to the screen with a resolution of 448 X 448. Using a megafunction of a 2 port ram called the frame buffer we were able to hold the current color for each pixel in our display in the onboard ram of the FPGA. This also allowed us to quickly read the correct color of a pixel when the VGA driver requested it and to updated the contents of the frame buffer whenever the a successful SPI transaction occured.

At the end of the lab we used our UNO to generate a square pixel and maneuver it around the screen using 4 push buttons.

#### Acoustic Team
