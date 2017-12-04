/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define IR_PIN A5
#define DETECT_THRESH 300

#define LOG_OUT 1 // use the log output function
#define FFT_N 128 // set to 256 point fft
#define FREQ_BIN 146
#define KHZ_7_START 47
#define KHZ_12_START 80
#define KHZ_17_START 113

#define KHZ_RANGE 5

#include <FFT.h> // include the library


void setup() {
  Serial.begin(115200); // use the serial port
  pinMode(IR_PIN, INPUT);
  /*
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  */
}

void loop() {
  unsigned long res = getAvg();
  Serial.println(res);
  uint8_t freq = getFreq();
  if (res >= DETECT_THRESH) {
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
    
  }
  
  delay(1000);
 
}


uint8_t getFreq()
{
  int tim_old = TIMSK0;
  int adc_old = ADCSRA;
  int admux_old = ADMUX;
  int didr_old = DIDR0;
  
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  
   cli();  // UDRE interrupt slows this way down on arduino1.0
    byte freq = 0;
    byte test = 0;
    for (int i = 0 ; i < 256 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();

    int khz_7 = fft_log_out[KHZ_7_START]; 
    int khz_12 = fft_log_out[KHZ_12_START]; 
    int khz_17 = fft_log_out[KHZ_17_START];
    /*
    Serial.print("7KHZ: ");
    Serial.println(khz_7);
     Serial.print("12KHZ: ");
    Serial.println(khz_12);
     Serial.print("17KHZ: ");
    Serial.println(khz_17);
    */
  TIMSK0 = tim_old; // turn off timer0 for lower jitter
  ADCSRA = adc_old; // set the adc to free running mode
  ADMUX = admux_old; // use adc0
  DIDR0 = didr_old; 

  
        Serial.println("start");
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.print(i);
      Serial.print(": ");
      Serial.println(fft_log_out[i]); // send out the data
    }

    if (khz_17 > khz_12 && khz_17 > khz_7) {
      return 17;
    } else if (khz_12 > khz_7 && khz_12 > khz_17) {
      return 12;
    } else {
      return 7;
    }

    
    
    
 
}


unsigned long getAvg()
{
  unsigned long ret = 0;
  for (int i = 0; i < 10; i++)
  {
    ret += analogRead(IR_PIN);
    delay(1);
  }

  return ret / 10;
}
