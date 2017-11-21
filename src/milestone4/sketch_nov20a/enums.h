// Enums
typedef enum State {
  NO_LINE,
  ON_LINE,
  PASSED_LINE,
} state_t;

typedef enum StateDir {
  NEGX = 0,
  NEGY = 1,
  POSX = 2,
  POSY = 3
} state_dir_t;

typedef enum Boundary {
  LOW_TREASURE = 5,
  MID_TREASURE = 4,
  HIGH_TREASURE = 3,
  WALL = 2,
  NOTHING = 1,
  UNKNOWN = 0
} boundary_t;
