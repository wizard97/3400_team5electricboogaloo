#include <Servo.h>
// 950, black, 650 white
#define FRONT_LEFT_IR A0
#define FRONT_RIGHT_IR A1
#define OUT_LEFT_IR A2
#define OUT_RIGHT_IR A3
#define LEFT_SERVO 10
#define RIGHT_SERVO 9

#define THRESHOLD 800

typedef enum State
{
  STATE_LINE_FOLLOW,
  STATE_INIT_TURN_LEFT,
  STATE_TURNING_LEFT,
  STATE_INIT_TURN_RIGHT,
  STATE_TURNING_RIGHT,
} state_t;

typedef enum StateWideIR
{
  SWIR_NO_LINE,
  SWIR_ON_LINE,
  SWIR_PASSED_LINE,
} state_wideir_t;


Servo left_servo;
Servo right_servo;

static state_t state = STATE_LINE_FOLLOW;
static state_wideir_t wideir_state = SWIR_NO_LINE;
static int line_count = 0;

int no_turn = 180;
int full_left = 0;
int full_right = 360;

void setup() {
  // put your setup code here, to run once:
  pinMode(FRONT_LEFT_IR, INPUT);
  pinMode(FRONT_RIGHT_IR, INPUT);
  pinMode(OUT_LEFT_IR, INPUT);
  pinMode(OUT_RIGHT_IR, INPUT);
  //pinMode(LEFT_SERVO, OUTPUT);
  //pinMode(RIGHT_SERVO, output);

  left_servo.attach(LEFT_SERVO);
  right_servo.attach(RIGHT_SERVO);

  Serial.begin(115200);
}

void loop() {
  unsigned fleft_ir = analogRead(FRONT_LEFT_IR);
  unsigned fright_ir = analogRead(FRONT_RIGHT_IR);
  unsigned oleft_ir = analogRead(OUT_LEFT_IR);
  unsigned oright_ir = analogRead(OUT_RIGHT_IR);
  // put your main code here, to run repeatedly:

  switch (wideir_state)
  {
  case (SWIR_NO_LINE): default:
      if (oleft_ir > THRESHOLD && oright_ir > THRESHOLD) {
        wideir_state = SWIR_ON_LINE;
      }
      break;

    case (SWIR_ON_LINE):
      if (oleft_ir < THRESHOLD && oright_ir < THRESHOLD) {
        wideir_state = SWIR_PASSED_LINE;
        line_count++;
      }
      break;

    case (SWIR_PASSED_LINE):
      wideir_state = SWIR_NO_LINE;
      break;
  }

  switch (state)
  {
  case STATE_LINE_FOLLOW: default:
      if (wideir_state == SWIR_PASSED_LINE && line_count % 4) {
        if (line_count % 8 <= 3 ) {
          Serial.println("Turning right");
          turnRightIP();
        } else if (line_count % 8 > 4) {
          Serial.println("Turning left");
          turnLeftIP();
        }
      } else if (fleft_ir < THRESHOLD) { //left sensor on white
        //speed up left, and/or slow down right
        turnRight(10);
      } else if (fright_ir < THRESHOLD) { //right sensor on white
        turnLeft(10);
      } else {
        forward(10);
      }
      break;
  }





  Serial.print("IR Left: ");
  Serial.println(fleft_ir);
  Serial.print("IR Right: ");
  Serial.println(fright_ir);
  delay(50);
}


void forward(uint8_t speed)
{
  int right = map(speed, 0, 100, 90, 0);
  int left = map(speed, 0, 100, 90, 180);
  right_servo.write(right);
  left_servo.write(left);
}


void turnRight(uint8_t speed)
{
  int left = map(speed, 0, 100, 90, 180);
  right_servo.write(90);
  left_servo.write(left);
}


void turnLeft(uint8_t speed)
{
  int right = map(speed, 0, 100, 90, 0);
  right_servo.write(right);
  left_servo.write(90);
}


void turnRightIP()
{
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
}

void turnLeftIP()
{
  int speed = 50;
  int val = map(speed, 0, 100, 90, 0);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
}


void stopMotors()
{
  right_servo.write(90);
  left_servo.write(90);
}

