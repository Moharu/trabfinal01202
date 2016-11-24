/*
    Struct definition of all hostile entities in the game
*/
typedef struct pipe {
    int active;
    int scored;
    float hPosition; // The pipe's x position
    float hWidth; // The total pipe's width
    float gap[2]; // y coordinates of the pipe's gap
} Pipe;
