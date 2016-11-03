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
	int gameEnded;
    Physics physics;
    Pipe pipe; // Description of all current pipes
} GameState;

GameState gameReducer(GameState, Action);
GameState pipeReducer(GameState, Pipe);
