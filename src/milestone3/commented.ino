#include <Servo.h>

#define MAX_STACK_SIZE 20
#define X_DIM 4
#define Y_DIM 5

#define LEFT_SERVO 6
#define RIGHT_SERVO 5
#define IN_LEFT_IR A0
#define IN_RIGHT_IR A1
#define OUT_LEFT_IR A2
#define OUT_RIGHT_IR A3
#define LINE_THRESHOLD 850 // black = 950, white = 650

#define FRONT_WALL A0
#define LEFT_WALL A1
#define RIGHT_WALL A2
#define WALL_THRESHOLD 150

// Structs
typedef struct Node {
  bool visited; // unvisited = false, visited = true
  boundary_t negX, posX, negY, posY;
} node_t;

typedef struct Walls {
  bool left_wall;
  bool front_wall;
  bool right_wall;
} walls_t;

// Enums 
typedef enum Boundary {
  TREASURE,
  WALL,
  NOTHING,
  UNKNOWN
} boundary_t;

typedef enum StateDir {
  NEGX = 0,
  NEGY = 1,
  POSX = 2,
  POSY = 3
} state_dir_t;

typedef enum State {
  NO_LINE,
  ON_LINE,
  PASSED_LINE,
} state_t;

// Variables
Servo left_servo;
Servo right_servo;
node_t grid[Y_DIM][X_DIM]; // yidx*X_DIM*sizeof(node_t) + xidx*sizeof(node_t)

state_t state = NO_LINE;
state_dir_t dir = POSY;
int x_coord = 0;
int y_coord = 0;

void setup() {
  left_servo.attach(LEFT_SERVO);
  right_servo.attach(RIGHT_SERVO);
  pinMode(IN_LEFT_IR, INPUT);
  pinMode(IN_RIGHT_IR, INPUT);
  pinMode(OUT_LEFT_IR, INPUT);
  pinMode(OUT_RIGHT_IR, INPUT);

  node_t *visited[MAX_STACK_SIZE] = {NULL};
  node_t *frontier[MAX_STACK_SIZE] = {NULL};
  gridInit();

  Serial.begin(115200);
}

void loop() {
  int ileft_ir = analogRead(IN_LEFT_IR);
  int iright_ir = analogRead(IN_RIGHT_IR);
  int oleft_ir = analogRead(OUT_LEFT_IR);
  int oright_ir = analogRead(OUT_RIGHT_IR);
  int delta_dir = ileft_ir - iright_ir;
  
  int front_dist = analogRead(FRONT_WALL);
  int left_dist = analogRead(LEFT_WALL);
  int right_dist = analogRead(RIGHT_WALL);

  switch(state) {
    case(NO_LINE): default:
      if (oleft_ir > LINE_THRESHOLD && oright_ir > LINE_THRESHOLD) {
        state = ON_LINE;
      }
      break;

    case(ON_LINE):
      if (oleft_ir < LINE_THRESHOLD && oright_ir < LINE_THRESHOLD) {
        state = PASSED_LINE;
      }
      break;

    case(PASSED_LINE):
      x_coord = updateXCoord(dir, x_coord);
      y_coord = updateYCoord(dir, y_coord);
      pushStack(&grid[y_coord][x_coord], visited);
      
      walls_t walls = checkWalls(front_dist, left_dist, right_dist);
      int front_wall_x = -1;
      int front_wall_y = -1;
      int left_wall_x = -1;
      int left_wall_y = -1;
      int right_wall_x = -1;
      int right_wall_y = -1;
      if(!walls.front_wall) {
        front_wall_x = updateXCoord(dir, x_coord);
        front_wall_y = updateYCoord(dir, y_coord);
        front_open = checkOpen(front_wall_x, front_wall_y);
        if(front_open) pushStack(&grid[front_wall_y][front_wall_x], frontier);
      }
      //else update boundary in correct direction
      if(!walls.left_wall) {
        int left_wall_dir = (dir + 3)%4;
        left_wall_x = updateXCoord(left_wall_dir, x_coord);
        left_wall_y = updateYCoord(left_wall_dir, y_coord);
        left_open = checkOpen(left_wall_x, left_wall_y);
        if(left_open) pushStack(&grid[left_wall_y][left_wall_x], frontier);
      }
      //else update boundary in correct direction
      if(!walls.right_wall) {
        int right_wall_dir = (dir + 1)%4;
        right_wall_x = updateXCoord(right_wall_dir, x_coord);
        right_wall_y = updateYCoord(right_wall_dir, y_coord);
        right_open = checkOpen(right_wall_x, right_wall_y);
        if(right_open) pushStack(&grid[right_wall_y][right_wall_x], frontier);
      }
      //else update boundary in correct direction

      
     /* if (frontier == NULL){
      *   //robot stop
      *   //indicate done on graphics and play sound
      *  }
      */
      node_t popped_frontier = popStack(frontier);
      //check &grid[left_wall_y][left_wall_x]? Need to check all asjacent nodes against popped value of frontier
      //check front value, right value, then left value and if none available check back through visited nodes to find path to next coord
      if((left_wall_x == popped) or (right_wall_x) or ())
      else {
        node_t popped_visited = popStack(visited);
      }

      state = NO_LINE;
      break;
  }

  // line following
  int maxs = map(FORWARD_SPEED, 0, 100, 90, 0);
  
  int right = (90-maxs) - delta_dir/LINE_FOLLOW_P;
  int left = (180-maxs) - delta_dir/LINE_FOLLOW_P;

  right = (right < 0) ? 0 : ((right > 90) ? 90 : right); 
  left = left > 180 ? 180 : left < 90 ? 90 : left;
   
  right_servo.write(right);
  left_servo.write(left);
  // end line following

  delay(50);
}

// Setup helper functions
void gridInit() {
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

// Helper functions to move the robot
void turnRight() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 180);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
  updateDir(dir, 2);
}

void turnLeft() {
  int speed = 50;
  int val = map(speed, 0, 100, 90, 0);
  right_servo.write(val);
  left_servo.write(val);
  delay(600);
  stopMotors();
  updateDir(dir, 0);
}

void stopMotors() {
  right_servo.write(90);
  left_servo.write(90);
}

// Helper functions to update the position and orientation
int updateXCoord(state_dir_t &dir, int x) {
  switch(dir) {
    case(NEGY): default:
      return x;
      break;
    case(POSY):
      return x;
      break;
    case(POSX):
      return x+1;
      break;
    case(NEGX):
      return x-1;
      break;
  }
}

int updateYCoord(state_dir_t &dir, int y) {
  switch(dir) {
    case(NEGY): default:
      return y-1;
      break;
    case(POSY):
      return y+1;
      break;
    case(POSX):
      return y;
      break;
    case(NEGX):
      return y;
      break;
  }
}

void updateDir(state_dir_t &dir, int lfr) { // lr: 0 = left, 1 = front, 2 = right
  switch(dir) {
    case(NEGY): default:
      if(lfr == 2) dir = POSX;
      else if(!lfr) dir = NEGX;
      break;
    case(POSY):
      if(lfr == 2) dir = NEGX;
      else if(!lfr) dir = POSX;
      break;
    case(POSX):
      if(lfr == 2) dir = POSY;
      else if(!lfr) dir = NEGY;
      break;
    case(NEGX):
      if(lfr == 2) dir = NEGY;
      else if(!lfr) dir = POSY;
      break;
  }
}

// Helper functions to check the walls
walls_t checkWalls(front_dist, left_dist, right_dist) {
  //int rot = (4 - dir)%4;

  walls_t walls;
  if(front_dist > WALL_THRESHOLD) walls.front_wall = 1;
  else walls.front_wall = 0;
  if(left_dist > WALL_THRESHOLD) walls.left_wall = 1;
  else walls.left_wall = 0;
  if(right_dist > WALL_THRESHOLD) walls.right_wall = 1;
  else walls.right_wall = 0;


if 





  /*walls.front_wall = walls_array[(0+rot)%4];
  walls.left_wall = walls_array[(3+rot)%4];
  walls.right_wall = walls_array[(3+rot)%4];*/

  return walls;
}

// Other helper functions
bool checkOpen(x, y) {
  if(x < 0 or x > 3 or y < 0 or y > 4) return false;
  if(grid[y][x].visited) return false;
  return true;
}

/*void lookup_node(uint8_t &xidx, uint8_t &yidx, node_t *node_addr) {
  yidx = ((node_t*)grid - node_addr)/(X_DIM*sizeof(node_t));
  xidx = (((node_t*)grid - node_addr)%(X_DIM*sizeof(node_t)))/sizeof(node_t);
}*/
