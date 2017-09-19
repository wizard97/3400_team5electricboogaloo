#define FRONT_WALL A0
#define LEFT_WALL A1
#define RIGHT_WALL A2
//#define THRESHOLD 

void setup() {
  pinMode(FRONT_WALL, INPUT);
  pinMode(LEFT_WALL, INPUT);
  pinMode(RIGHT_WALL, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115000);
}

void loop() {
  int front_dist = analogRead(FRONT_WALL);
  int left_dist = analogRead(LEFT_WALL);
  int right_dist = analogRead(RIGHT_WALL);
  
  Serial.print("Front: ");
  Serial.println(front_dist);
  Serial.print("Left: ");
  Serial.println(left_dist);
  Serial.print("Right: ");
  Serial.println(right_dist);

  if(front_dist > THRESHOLD) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);]
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
  if(left_dist > THRESHOLD) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);]
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
  if(right_dist > THRESHOLD) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);]
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}

