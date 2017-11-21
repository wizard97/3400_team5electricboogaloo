#include <Servo.h>
#include <SPI.h>
#include "enums.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Parameters
#define MAX_STACK_SIZE 20
#define X_DIM 5
#define Y_DIM 4

#define LEFT_SERVO 6
#define RIGHT_SERVO 5
#define IN_LEFT_IR A0
#define IN_RIGHT_IR A1
#define OUT_LEFT_IR A2
#define LINE_THRESHOLD 850 // white = 650, black = 950
#define LINE_FOLLOW_P 2
#define FORWARD_SPEED 10

#define WALL_THRESHOLD 150

#define MUX_OUT A3
#define MUX_S0 2
#define MUX_S1 3
#define MUX_S2 4

// Structs
typedef struct Node {
  bool visited; // unvisited = false, visited = true
  int x, y;
  boundary_t left, front, right;
} node_t;

typedef struct Walls {
  bool left_wall, front_wall, right_wall;
} walls_t;

// Global variables
node_t grid[Y_DIM][X_DIM];
node_t *visited[MAX_STACK_SIZE] = {NULL};
node_t *frontier[MAX_STACK_SIZE] = {NULL};

state_t state = PASSED_LINE;
state_dir_t dir = POSY;
int x_coord = 0;
int y_coord = -1;

Servo left_servo, right_servo;

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000010LL, 0x0000000011LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup() {
  left_servo.attach(LEFT_SERVO);
  right_servo.attach(RIGHT_SERVO);
  pinMode(IN_LEFT_IR, INPUT);
  pinMode(IN_RIGHT_IR, INPUT);
  pinMode(OUT_LEFT_IR, INPUT);
  pinMode(MUX_OUT, INPUT);
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);

  gridInit();

  Serial.begin(115200);
  //Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  if( role == role_ping_out ) {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  } else {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  radio.startListening();
  radio.printDetails();

  //Serial.begin(115200);
}

void loop() {
  int ileft_ir = analogRead(IN_LEFT_IR);
  int iright_ir = analogRead(IN_RIGHT_IR);
  int oleft_ir = analogRead(OUT_LEFT_IR);
  digitalWrite(MUX_S2, LOW);
  digitalWrite(MUX_S1, LOW);
  digitalWrite(MUX_S0, LOW);
  int front_dist = analogRead(MUX_OUT);
  digitalWrite(MUX_S2, LOW);
  digitalWrite(MUX_S1, LOW);
  digitalWrite(MUX_S0, HIGH);
  int left_dist = analogRead(MUX_OUT);
  digitalWrite(MUX_S2, LOW);
  digitalWrite(MUX_S1, HIGH);
  digitalWrite(MUX_S0, LOW);
  int right_dist = analogRead(MUX_OUT);
  
  int maxs = map(FORWARD_SPEED, 0, 100, 90, 0);
  int delta_dir = ileft_ir - iright_ir;
  int left = (180-maxs) - delta_dir/LINE_FOLLOW_P;
  int right = (90-maxs) - delta_dir/LINE_FOLLOW_P;
  left = left > 180 ? 180 : (left < 90 ? 90 : left);
  right = right < 0 ? 0 : (right > 90 ? 90 : right); 

  switch(state) {
    case(NO_LINE): default:
      left_servo.write(left);
      right_servo.write(right);

      if(oleft_ir > LINE_THRESHOLD) state = ON_LINE;
      break;

    case(ON_LINE):
      left_servo.write(left);
      right_servo.write(right);
      
      if (oleft_ir < LINE_THRESHOLD) state = PASSED_LINE;
      break;

    case(PASSED_LINE):
      x_coord = updateXCoord(dir, x_coord);
      y_coord = updateYCoord(dir, y_coord);
      grid[y_coord][x_coord].visited = true;
      
      walls_t walls = checkWalls(front_dist, left_dist, right_dist);
      state_dir_t left_dir, right_dir;
      int front_x = -1;
      int front_y = -1;
      int left_x = -1;
      int left_y = -1;
      int right_x = -1;
      int right_y = -1;
      if(!walls.right_wall) {
        grid[y_coord][x_coord].right = NOTHING;
        right_dir = (state_dir_t) ((dir + 1)%4);
        right_x = updateXCoord(right_dir, x_coord);
        right_y = updateYCoord(right_dir, y_coord);
        if(checkOpen(right_x, right_y)) pushStack(&grid[right_y][right_x], frontier);
      } else {
        //treasure detection
        grid[y_coord][x_coord].right = WALL;
      } if(!walls.left_wall) {
        grid[y_coord][x_coord].left = NOTHING;
        left_dir = (state_dir_t) ((dir + 3)%4);
        left_x = updateXCoord(left_dir, x_coord);
        left_y = updateYCoord(left_dir, y_coord);
        if(checkOpen(left_x, left_y)) pushStack(&grid[left_y][left_x], frontier);
      } else {
        //treasure detection
        grid[y_coord][x_coord].left = WALL;
      } if(!walls.front_wall) {
        grid[y_coord][x_coord].front = NOTHING;
        front_x = updateXCoord(dir, x_coord);
        front_y = updateYCoord(dir, y_coord);
        if(checkOpen(front_x, front_y)) pushStack(&grid[front_y][front_x], frontier);
      } else {
        //treasure detection
        grid[y_coord][x_coord].front = WALL;
      }

      node_t *popped_frontier = popStack(frontier);
      while(popped_frontier->visited) {
        popped_frontier = popStack(frontier);
      }
      if(popped_frontier == NULL) {
        stopMotors();
        //display 'DONE' on screen
        //play song
        while(1);
      } else {
        if(front_x == popped_frontier->x && front_y == popped_frontier->y) {
          pushStack(&grid[y_coord][x_coord], visited);
        } else if(left_x == popped_frontier->x && left_y == popped_frontier->y) {
          pushStack(&grid[y_coord][x_coord], visited);
          turnLeft();
        } else if(right_x == popped_frontier->x && right_y == popped_frontier->y) {
          pushStack(&grid[y_coord][x_coord], visited);
          turnRight();
        } else {
          pushStack(popped_frontier, frontier);
          node_t *popped_visited = popStack(visited);
          left_dir = (state_dir_t) ((dir + 3)%4);
          left_x = updateXCoord(left_dir, x_coord);
          left_y = updateYCoord(left_dir, y_coord);
          right_dir = (state_dir_t) ((dir + 1)%4);
          right_x = updateXCoord(right_dir, x_coord);
          right_y = updateYCoord(right_dir, y_coord);
          state_dir_t back_dir = (state_dir_t) ((dir + 2)%4);
          int back_x = updateXCoord(back_dir, x_coord);
          int back_y = updateYCoord(back_dir, y_coord);
          if(left_x == popped_visited->x && left_y == popped_visited->y) turnLeft();
          if(right_x == popped_visited->x && right_y == popped_visited->y) turnRight();
          if(back_x == popped_visited->x && back_y == popped_visited->y) reverse();
        }
      } transmit(x_coord, y_coord, dir);
      state = NO_LINE;
      break;
  } delay(50);
}

// Radio helper functions
void transmit(x, y, next_dir) {
  if(role == role_ping_out) {
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned char data;

    unsigned char transmit_x = x;
    unsigned char transmit_y = y;
    unsigned char transmit_dir = next_dir;
    unsigned char negX_bound = grid[y][x].negX;
    unsigned char posX_bound = grid[y][x].posX;
    unsigned char negY_bound = grid[y][x].negY;
    unsigned char posY_bound = grid[y][x].posY;

    data = negX_bound << 16 | posX_bound << 13 | negY_bound << 10 | posY_bound << 7 | transmit_dir << 5 | transmit_x << 2 | transmit_y;
    //bound_data = negX_bound << 16 | posX_bound << 13 | negY_bound << 10 | posY_bound << 7;
    printf("Now sending position and state data.\n");
    bool ok = radio.write( &data, sizeof(data) );

    if(ok) printf("ok...sent payload %d, suck it trebek ",data);
    else printf("failed.\n\r");

    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while( ! radio.available() && ! timeout ) {
      if (millis() - started_waiting_at > 200 )
        timeout = true;
    }

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }
}

// Setup helper functions
void gridInit() {
  for (int i = 0; i < Y_DIM; i++) {
    for (int j = 0; j < X_DIM; j++) {
      grid[i][j].visited = false;
      grid[i][j].x = j;
      grid[i][j].y = i;
      grid[i][j].left = UNKNOWN;
      grid[i][j].front = UNKNOWN;
      grid[i][j].right = UNKNOWN;
    }
  }
}

// Stack helper functions
bool pushStack(node_t *n, node_t **stack) {
  for (uint8_t i = 0; i < MAX_STACK_SIZE; i++) {
    if (!stack[i]) {
      stack[i] = n;
      return true;
    }
  } return false;
}

node_t *popStack(node_t **stack) {
  node_t *prev = stack[0];
  for (uint8_t i = 1; i < MAX_STACK_SIZE; i++) {
    if (!stack[i]) {
      stack[i-1] = NULL;
      return prev;
    }
    prev = stack[i];
  } return prev;
}

// Robot helper functions
void turnLeft() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 0);
  left_servo.write(val);
  right_servo.write(val);
  delay(200);
  while(analogRead(IN_LEFT_IR) < LINE_THRESHOLD || analogRead(IN_RIGHT_IR) < LINE_THRESHOLD);
  dir = updateDir(dir, 0);
}

void turnRight() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  left_servo.write(val);
  right_servo.write(val);
  delay(200);
  while(analogRead(IN_LEFT_IR) < LINE_THRESHOLD || analogRead(IN_RIGHT_IR) < LINE_THRESHOLD);
  dir = updateDir(dir, 2);
}

void reverse() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  left_servo.write(val);
  right_servo.write(val);
  delay(1300);
  dir = updateDir(dir, 2);
  dir = updateDir(dir, 2);
}

void stopMotors() {
  left_servo.write(90);
  right_servo.write(90);
}

// Helper functions to update position and orientation
int updateXCoord(state_dir_t dir, int x) {
  switch(dir) {
    case(NEGX): default:
      return (x-1);
      break;
    case(POSX):
      return (x+1);
      break;
    case(NEGY):
      return x;
      break;
    case(POSY):
      return x;
      break;
  }
}

int updateYCoord(state_dir_t dir, int y) {
  switch(dir) {
    case(NEGX): default:
      return y;
      break;
    case(POSX):
      return y;
      break;
    case(NEGY):
      return (y-1);
      break;
    case(POSY):
      return (y+1);
      break;
  }
}

state_dir_t updateDir(state_dir_t dir, int lfr) { // lfr: 0 = left, 1 = front, 2 = right
  switch(dir) {
    case(NEGX): default:
      if(lfr == 2) return NEGY;
      else if(!lfr) return POSY;
      break;
    case(POSX):
      if(lfr == 2) return POSY;
      else if(!lfr) return NEGY;
      break;
    case(NEGY):
      if(lfr == 2) return POSX;
      else if(!lfr) return NEGX;
      break;
    case(POSY):
      if(lfr == 2) return NEGX;
      else if(!lfr) return POSX;
      break;
  }
}

// Helper functions to check walls
walls_t checkWalls(int front_dist, int left_dist, int right_dist) {
  walls_t walls;
  if(front_dist > WALL_THRESHOLD) walls.front_wall = 1;
  else walls.front_wall = 0;
  if(left_dist > WALL_THRESHOLD) walls.left_wall = 1;
  else walls.left_wall = 0;
  if(right_dist > WALL_THRESHOLD) walls.right_wall = 1;
  else walls.right_wall = 0;
  return walls;
}

bool checkOpen(int x, int y) {
  if(x < 0 or x > 4 or y < 0 or y > 3) return false;
  if(grid[y][x].visited) return false;
  return true;
}
