#include "Actions.h"
#include "Hostile.h"

typedef struct physics {
    float height; // current vertical position
    float velocity; // current velocity
    float maxHeight; // Max height
    float gravity; // Gravity Acceleration
    float elapsedTime; // Elapsed time between last frame (ms)
    float hVelocity; // Horizontal velocity
    float hPosition; // Horizontal position
} Physics;

// Game State structure
typedef struct gamestate {
    Physics physics;
    Pipe pipes[10]; // Description of all current pipes
} GameState;

GameState gameReducer(GameState, Action);
