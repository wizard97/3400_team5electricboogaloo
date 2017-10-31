#include <Servo.h>

#define MAX_STACK_SIZE 20
// black = 950, white = 650
#define Y_DIM 5
#define X_DIM 4
#define FRONT_LEFT_IR A0
#define FRONT_RIGHT_IR A1
#define OUT_LEFT_IR A2
#define OUT_RIGHT_IR A3
#define LEFT_SERVO 6
#define RIGHT_SERVO 5

#define LINE_THRESHOLD 850

#define FRONT_WALL A0
#define LEFT_WALL A1
#define RIGHT_WALL A2
#define WALL_THRESHOLD 150

#define LINE_FOLLOW_P 2
#define FORWARD_SPEED 50

typedef enum Boundary {
  TREASURE,
  WALL,
  NOTHING,
  UNKNOWN
} boundary_t;

typedef struct Node {
  bool visited; // unvisited = False, visited = True
  boundary_t negX, posX, negY, posY;
} node_t;

typedef struct Walls {
  bool left_wall;
  bool front_wall;
  bool right_wall;
} walls_t;

node_t *visited[MAX_STACK_SIZE] = {NULL};
node_t *frontier[MAX_STACK_SIZE] = {NULL};

struct FrontierStack {
  Node &head;
};

struct VisitedStack {
  Node &head;
};

typedef enum StateDir {
  NEGX = 0,
  NEGY = 1,
  POSX = 2,
  POSY = 3
} state_dir_t;

typedef enum StateWideIR
{
  SWIR_NO_LINE,
  SWIR_ON_LINE,
  SWIR_PASSED_LINE,
} state_wideir_t;

Servo left_servo;
Servo right_servo;

node_t grid[Y_DIM][X_DIM]; // yidx*X_DIM*sizeof(node_t)+ xidx*sizeof(node_t)


//static state_t state = STATE_LINE_FOLLOW;
static state_wideir_t wideir_state = SWIR_NO_LINE;

int x_coord = 0;
int y_coord = 0;

state_dir_t dir = POSY;

void setup() {
  // put your setup code here, to run once:
  pinMode(FRONT_LEFT_IR, INPUT);
  pinMode(FRONT_RIGHT_IR, INPUT);
  pinMode(OUT_LEFT_IR, INPUT);
  pinMode(OUT_RIGHT_IR, INPUT);

  left_servo.attach(LEFT_SERVO);
  right_servo.attach(RIGHT_SERVO);

  grid_init();

  Serial.begin(115200);
}

void loop() {
  int fleft_ir = analogRead(FRONT_LEFT_IR);
  int fright_ir = analogRead(FRONT_RIGHT_IR);

  int delta_dir = fleft_ir - fright_ir;
  int oleft_ir = analogRead(OUT_LEFT_IR);
  int oright_ir = analogRead(OUT_RIGHT_IR);
  
  int front_dist = analogRead(FRONT_WALL);
  int left_dist = analogRead(LEFT_WALL);
  int right_dist = analogRead(RIGHT_WALL);

  switch(wideir_state) {
    case (SWIR_NO_LINE): default:
      if (oleft_ir > LINE_THRESHOLD && oright_ir > LINE_THRESHOLD) {
        wideir_state = SWIR_ON_LINE;
      }
      break;

    case (SWIR_ON_LINE):
      if (oleft_ir < LINE_THRESHOLD && oright_ir < LINE_THRESHOLD) {
        wideir_state = SWIR_PASSED_LINE;
      }
      break;

    case (SWIR_PASSED_LINE):
      push_stack(&grid[y_coord][x_coord], visited);
      grid[y_coord][x_coord]
      walls_t walls = check_walls(dir, front_dist, left_dist, right_dist);
      if (!walls.left_wall) 
      
      update_coord(dir);
      wideir_state = SWIR_NO_LINE;
      break;
  }

  int maxs = map(FORWARD_SPEED, 0, 100, 90, 0);
  
  int right = (90-maxs) - delta_dir/LINE_FOLLOW_P;
  int left = (180-maxs) - delta_dir/LINE_FOLLOW_P;

  right = (right < 0) ? 0 : ((right > 90) ? 90 : right); 
  left = left > 180 ? 180 : left < 90 ? 90 : left;
   
  right_servo.write(right);
  left_servo.write(left);

  delay(50);
}


walls_t check_walls(robot_dir, front_dist, left_dist, right_dist) {
  walls_t walls;
  bool wallsens[4]
  if(left_dist > WALL_THRESHOLD) wallsens[0] = true;
  if(front_dist > WALL_THRESHOLD) wallsens[1] = true;
  if(right_dist > WALL_THRESHOLD) wallsens[2] = true;
  wallsens[3] = true;

  int rot = (4 - dir)%4;

  walls.front_wall = walls[(0+rot)%4];
  walls.left_wall = walls[(3+rot)%4];
  walls.right_wall = walls[(3+rot)%4];
  
   
  
  
  walls_t walls_array;
  walls_array.front_wall = 0;
  walls_array.left_wall = 0;
  walls_array.right_wall = 0;
  if(front_dist > WALL_THRESHOLD) walls_array.front_wall = 1;
  if(left_dist > WALL_THRESHOLD) walls_array.left_wall = 1;
  if(right_dist > WALL_THRESHOLD) walls_array.right_wall = 1;
  return walls_array;
}

void lookup_node(uint8_t &xidx, uint8_t &yidx, node_t *node_addr)
{
  yidx= ((node_t*)grid - node_addr)/(X_DIM*sizeof(node_t));
  xidx= (((node_t*)grid - node_addr)%(X_DIM*sizeof(node_t)))/sizeof(node_t);
}



node_t *pop_stack(node_t **stack)
{
  node_t *prev = stack[0];
  for (uint8_t i=1; i < MAX_STACK_SIZE; i++) {
    if (!stack[i]) {
      stack[i-1] = NULL;
      return prev;
    }
    prev = stack[i];
  }

  return prev;
}



bool push_stack(node_t *n, node_t **stack)
{
  for (uint8_t i=0; i < MAX_STACK_SIZE; i++) {
    if (!stack[i]) {
      stack[i] = n;
      return true;
    }
  }

  return false;
}



void grid_init() {
  for (int i = 0; i < Y_DIM; i++) {
    for (int j = 0; j < X_DIM; j++) {
      grid[i][j].visited = false;
      grid[i][j].posX = UNKNOWN;
      grid[i][j].negX = UNKNOWN;
      grid[i][j].negY = UNKNOWN;
      grid[i][j].posY = UNKNOWN;
    }
  }
}

void turnRightIP() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
  update_dir(dir, 1);
}

void turnLeftIP()
{
  int speed = 50;
  int val = map(speed, 0, 100, 90, 0);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
  update_dir(dir, 0);
}


void stopMotors()
{
  right_servo.write(90);
  left_servo.write(90);
}

void update_dir(state_dir_t &dir, int lr) { // 0 = left, 1 = right
  switch(dir) {
    case(NEGY): default:
      if(lr) {
        dir = POSX;
      } else {
        dir = NEGX;
      }
      break;
    case(POSY):
      if(lr) {
        dir = NEGX;
      } else {
        dir = POSX;
      }
      break;
    case(POSX):
      if(lr) {
        dir = POSY;
      } else {
        dir = NEGY;
      }
      break;
    case (NEGX):
      if(lr) {
        dir = NEGY;
      } else {
        dir = POSY;
      }
      break;
  }
}

void update_coord(state_dir_t &dir) {
  switch(dir) {
    case(NEGY): default:
      y_coord -= 1;
      break;
    case(POSY):
      y_coord += 1;
      break;
    case(POSX):
      x_coord += 1;
      break;
    case (NEGX):
      x_coord -= 1;
      break;
  }
}
