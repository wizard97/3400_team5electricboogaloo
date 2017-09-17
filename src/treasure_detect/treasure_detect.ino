#define IR_PIN A0
#define DETECT_THRESH 20
#define LED_PIN 2

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long res = getAvg();
  if (res >= DETECT_THRESH) {
    Serial.print("Detection! Value: ");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.print("No detection! Value: ");
    digitalWrite(LED_PIN, LOW);
  }
  Serial.println(res);

  //delay(500);
}


unsigned long getAvg()
{
  unsigned long ret = 0;
  for (int i=0; i < 1000; i++)
  {
    ret += analogRead(IR_PIN);
    delayMicroseconds(100);
  }

  return ret /1000;
}
