# __*Milestone 2*__

### Objective
The objective of this milestone was to add treasure detection for varying frequencies and wall detection to our robot. We implemented treasure detection for 12 and 17 kHz frequencies in addition to the 7 kHz detection that was implemented in [Lab 2](Lab2Page.md). For the wall detection, we implemented a total of three short-range sensors (left, right, and front).

### Work Distribution
The entire team worked together on this milestone. Aaron, David, and Adam worked primarily on the treasure detection, while Christina, Kelsey, and Rajiv worked primarily on the wall detection.

### Helpful Links
* [Datasheet for the short-range infrared sensor](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0a41sk_e.pdf)
* [Datasheet for the long-range infrared sensor](https://www.sparkfun.com/datasheets/Sensors/Infrared/gp2y0a02yk_e.pdf)

### Lab Documentation

#### Materials
* Wall sensors
* Wood block
* Resistors
* Capacitors
* Op amp
* 2 9-volt batteries
* IR Sensor
* treasures

#### Varying Treasure Frequencies
We began by using our circuit and code from lab 2 for detecting a 7kHz treasure. We modified several values of the circuit so that we could have a band pass filter with a range of about 2kHz-20kHz. We then modified the code by adding a similar FFT to the code for our microphone. We made it so that an FFT was done on the output from the IR sensor, so that we could calculate the specific bin numbers of the 3 treasure values and distinguish them. We used those bins to alter a frequency variable and based on the detection and change of the variable, the monitor will print the correct value of the different treasures.

``` if (res >= DETECT_THRESH) {
    Serial.print("Detection! freq: ");
     uint8_t freq = getFreq();

    if (freq == 17) {
      Serial.println("17KHz");
    } else if (freq == 12) {
       Serial.println("12KHz");
    } else {
      Serial.println("7KHz");
    }
  } else {
    Serial.println("No detection!");
```
This snippet of code is the detection of a signal above the threshold and then the responding detection of different frequency treasures. 


See the video of our robot detecting the three distinct treasure frequencies [here]()!

#### Wall Detection
We began by testing both the short-range and the long-range wall sensors in order to determine which would be better for our robot in terms of how sensitive we would want to be in our wall detection. After determining that the short-range wall sensors would be better in the context of our maze (the long-range sensors reach their "peak" detection further away from the wall, and the detected values closer than that "peak" point are inaccurate), we tested our sensors again, this time to gauge a reasonable threshold at which to have our robot "register" the presence of a wall. For the purposes of this milestone, we used an external LED to indicate whether or not a wall is present. As shown in the code snippet below, when the analog input value from the wall sensor is above our measured threshold, the LED is turned on; when the input value is below the threshold, the LED is turned off.

```c++
int front_dist = analogRead(FRONT_WALL);

if(front_dist > THRESHOLD) {
  digitalWrite(FRONT_LED, HIGH);
} else {
  digitalWrite(FRONT_LED, LOW);
}
```

See [here](https://www.youtube.com/watch?v=70y1C5KFJqg) for a video demonstration of our robot's wall detection functionality!
