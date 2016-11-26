/*
    Struct definition of all hostile entities in the game
*/

#define TYPE_PIPE 1
#define TYPE_ENEMY 2

typedef struct pipe {
    float hWidth; // The total pipe's width
    float gap[2]; // y coordinates of the pipe's gap
} Pipe;

typedef struct enemy {
    float height;
    float hWidth;
} Enemy;

typedef struct hostile {
    int active;
    int scored;
    int type;
    float hPosition; // The pipe's x position
    Pipe pipe;
    Enemy enemy;
} Hostile;
