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
  LOW_TREASURE,
  MID_TREASURE,
  HIGH_TREASURE,
  WALL,
  NOTHING,
  UNKNOWN
} boundary_t;
