#include "Actions.h"

// Game State structure
typedef struct gamestate {
    float height; // current vertical position
    float velocity; // current velocity
    float maxHeight; // Max height
    float gravity; // Gravity Acceleration
    float elapsedTime; // Elapsed time between last frame (ms)
} GameState;

GameState gameReducer(GameState, Action);
