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
##### Setup
We began the assignment by examining the schematic for the FPGA so that we could find the GPIO pins we need to send the data to the screen. Since the FPGA's GPIO pins are not 5V tolerant, we created a simple level shifter using resistors for a voltage divider. Since the FPGA uses 3.3 V logic but the UNO's is 5V, we used a 1k ohm resistor and a 1.8k ohm resistor in a voltage divider configuration, with the 1.8K on the low side. The high side of the resistor pair was connected to the UNO's GPIO output, and the center tap was connected to the FPGA's GPIO. This drops the voltage from 5 V to roughly 3.3 V. This can easily be calculated from the voltage divider equation: Vout = 5*(1.8K)/(1.8K + 1K) = 3.21V.


##### VGA Working Principle
Since VGA relies on analog voltages, it is impossible to directly generate with the FPGA. Thus, the VGA adapter is needed to create analog voltages from digital output on the 8 pins that represent the 8-bit color. It acomplishes this by using a series of resistors, each with one end connected to a GPIO on the FPGA, and the other end all tied together going to the analog color out to the monitor.
This creates a crude ADC for each color channel controlled by a parallel bus. Note that the red and green channels are controlled by 3-bits and the blue only two bits. By outputting these digital bits on pins from the FPGA and connecting them to the corresponding resistor, it creates a crude parallel bus controlled 2-bit/3-bit DAC. When the bus pins are either set high or low, it creates a simple adjustable voltage divider that works by putting different resistors in parallel and/or making them on the high/low side of the voltage divier. For example, writing 3'b010 puts the 220 and 820 Ohm resistor in parallel on the low side and the 390 Ohm resistor on the high side. With simple calculations, one can see that the output voltage is roughly GPIO_LOGIC_LEVEL*0.308V. See the schematic and table below.

![alt text](Lab3pics/DAC2.png)

The resistors are chosen in such a way that that each digital step is as close to linear as possible.

TODO ONE OF YOU FINISH THESE!!!

Outputs Red and Green DAC

| Digital Value | Expected Analog Voltage (x GPIO_LOGIC_LEVEL V) | Actual Voltage (x GPIO_LOGIC_LEVEL V) |
| ------ |:------:| -----:|
| 0x0 | 0 | 0 |
| 0x1 | 0.143 | 0.146 |
| 0x2 | 0.286 | 0.308 |
| 0x3 | | |
| 0x4 | | |
| 0x5 | | |
| 0x6 | | |
| 0x7 | | |

Outputs Blue DAC

| Digital Value | Expected Analog Voltage (x GPIO_LOGIC_LEVEL V) | Actual Voltage (x GPIO_LOGIC_LEVEL V) |
| ------ |:------:| -----:|
| 0x0 | 0 | 0 |
| 0x2 | | |
| 0x3 | | |



##### Implementation
To send information to the FPGA we intially began implementing a parallel bus to send the data, but we realized this was not an ideal solution because it required way too many pins, requiring us to individually toggle the pins to update the values. It also required us to have a valid bit to avoid race conditions where the FPGA read an update midway through toggling pins. Here is our initial demo using 10 bits to select from 4 rectangles on the screen (2-bits), each with a particular color (8-bits).
[Random Colored Rectangles](https://youtu.be/jNI8C6o3gCc)

We decided to use a SPI driver to send the data. We found a template for a SPI online and modified it to accept three byte payloads with the first byte being the x-coordinate, the second being the y-coordinate, and the thrid being the color. The SPI gave us a significant performace increase over our parallel bus because we could use a 4 MHz clock to send information over it. The UNO has built in SPI hardware and operates on a 4 MHz clock so this gave us optimal speed while also minimizing the amount of wires we needed.

We then mapped a 120 by 120 grid to the screen with a resolution of 448 X 448. Using a megafunction of a 2 port ram called the frame buffer we were able to hold the current color for each pixel in our display in the onboard ram of the FPGA. This also allowed us to quickly read the correct color of a pixel when the VGA driver requested it and to updated the contents of the frame buffer whenever the a successful SPI transaction occured.

At the end of the lab we used our UNO to generate a square pixel and maneuver it around the screen using 4 push buttons.

#### Acoustic Team

##### Square Wave
We began by producing our square wave by toggling a GPIO pin at a frequency 440Hz, which corresponds to the note "A", the code can be seen here:

(INSERT RELEVANT CODE SNIPPET HERE)

The output to the oscilloscope looked like this: 
![alt text](Lab3pics/square_wave.png)

When hooked up to the speakers, this square wave sounded like [this](https://www.youtube.com/watch?v=5FbqoMoR8Ew)

##### Tune using DAC
Next, we generated a sin table in MatLab to generate the 8-bit binary numbers to be passed into the DAC, the code can be seen here:

(INSERT RELEVANT CODE SNIPPET HERE)

Then, we configured the pins to receive the proper bits from the sin table, resulting in the following output to the oscilloscope: ![alt text](Lab3pics/sdfdf.png)

We repeated this process for two other frequencies, 554.37Hz (C#), and 493.88Hz (B). Finally, we created a state machine to play each note for a half-second each to play the melody of [Mary Had a Little Lamb.](https://www.youtube.com/watch?v=XJbFZTgZO-0)

