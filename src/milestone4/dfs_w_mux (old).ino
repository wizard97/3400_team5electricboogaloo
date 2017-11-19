#include <Servo.h>
#include "enums.h"

// Parameters
#define MAX_STACK_SIZE 20
#define X_DIM 4
#define Y_DIM 5

#define LEFT_SERVO 6
#define RIGHT_SERVO 5
#define IN_LEFT_IR A0
#define IN_RIGHT_IR A1
#define OUT_LEFT_IR A2
#define LINE_THRESHOLD 850 // white = 650, black = 950650
#define LINE_FOLLOW_P 2
#define FORWARD_SPEED 10

#define FRONT_WALL A4
#define LEFT_WALL A5
#define RIGHT_WALL A3
#define WALL_THRESHOLD 150

//#define MUX_OUT A4
//#define MUX_S0 2
//#define MUX_S1 3
//#define MUX_S2 4

// Structs
typedef struct Node {
  bool visited; // unvisited = false, visited = true
  int x;
  int y;
  boundary_t negX, posX, negY, posY;
} node_t;

typedef struct Walls {
  bool left_wall;
  bool front_wall;
  bool right_wall;
} walls_t;

// Global variables
node_t grid[Y_DIM][X_DIM];
node_t *visited[MAX_STACK_SIZE] = {NULL};
node_t *frontier[MAX_STACK_SIZE] = {NULL};

state_t state = PASSED_LINE;
state_dir_t dir = POSY;
int x_coord = 0;
int y_coord = -1;

Servo left_servo;
Servo right_servo;

void setup() {
  left_servo.attach(LEFT_SERVO);
  right_servo.attach(RIGHT_SERVO);
  pinMode(IN_LEFT_IR, INPUT);
  pinMode(IN_RIGHT_IR, INPUT);
  pinMode(OUT_LEFT_IR, INPUT);
  pinMode(FRONT_WALL, INPUT);
  pinMode(LEFT_WALL, INPUT);
  //pinMode(MUX_OUT, INPUT);
  //pinMode(MUX_S0, OUTPUT);
  //pinMode(MUX_S1, OUTPUT);
  //pinMode(MUX_S2, OUTPUT);

  gridInit();

  Serial.begin(115200);
}

void loop() {
  int ileft_ir = analogRead(IN_LEFT_IR);
  int iright_ir = analogRead(IN_RIGHT_IR);
  int oleft_ir = analogRead(OUT_LEFT_IR);
  int front_dist = analogRead(FRONT_WALL);
  int left_dist = analogRead(LEFT_WALL);
  int right_dist = analogRead(RIGHT_WALL);
  //MUX_S0 = 0;
  //MUX_S1 = 0;
  //MUX_S2 = 0;
  //int right_dist = analogRead(MUX_OUT);
  
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
        right_dir = (state_dir_t) ((dir + 1)%4);
        right_x = updateXCoord(right_dir, x_coord);
        right_y = updateYCoord(right_dir, y_coord);
        if(checkOpen(right_x, right_y)) pushStack(&grid[right_y][right_x], frontier);
      } if(!walls.left_wall) {
        left_dir = (state_dir_t) ((dir + 3)%4);
        left_x = updateXCoord(left_dir, x_coord);
        left_y = updateYCoord(left_dir, y_coord);
        if(checkOpen(left_x, left_y)) pushStack(&grid[left_y][left_x], frontier);
      } if(!walls.front_wall) {
        front_x = updateXCoord(dir, x_coord);
        front_y = updateYCoord(dir, y_coord);
        if(checkOpen(front_x, front_y)) pushStack(&grid[front_y][front_x], frontier);
      }

      node_t *popped_frontier = popStack(frontier);
      while(popped_frontier->visited) {
        popped_frontier = popStack(frontier);
      }
      if(popped_frontier == NULL) {
        stopMotors();
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
      } state = NO_LINE;
      break;
  } delay(50);
}

// Setup helper functions
void gridInit() {
  for (int i = 0; i < Y_DIM; i++) {
    for (int j = 0; j < X_DIM; j++) {
      grid[i][j].visited = false;
      grid[i][j].x = j;
      grid[i][j].y = i;
      grid[i][j].negX = UNKNOWN;
      grid[i][j].posX = UNKNOWN;
      grid[i][j].negY = UNKNOWN;
      grid[i][j].posY = UNKNOWN;
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
  delay(500);
  stopMotors();
  dir = updateDir(dir, 0);
}

void turnRight() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  left_servo.write(val);
  right_servo.write(val);
  delay(600);
  stopMotors();
  dir = updateDir(dir, 2);
}

void reverse() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  left_servo.write(val);
  right_servo.write(val);
  delay(1300);
  stopMotors();
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
  if(x < 0 or x > 3 or y < 0 or y > 4) return false;
  if(grid[y][x].visited) return false;
  return true;
}
