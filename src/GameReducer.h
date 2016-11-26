/*
    State structure definitions and GameReducer function prototypes
*/
#include "Actions.h"
#include "Hostile.h"
#include <stdlib.h>
#include "GameConstants.h"

typedef struct physics {
    float height; // current vertical position
    float velocity; // current velocity
    float maxHeight; // Max height
    float gravity; // Gravity Acceleration
    float elapsedTime; // Elapsed time between last frame (ms)
    float hVelocity; // Horizontal velocity
    float hPosition; // Horizontal position
} Physics;

// Projectile (expert mode only)
typedef struct projectile {
    int active;
    float hPosition;
    float hVelocity;
    float height;
} Projectile;

// Game State structure
typedef struct gamestate {
    int points;
	int gameEnded;
    int spawnEnemies;
    Physics physics;
    Hostile hostile[MAX_HOSTILES]; // Description of all current hostile units
    Projectile projectile[MAX_PROJECTILES]; // Description of current projectiles
} GameState;

GameState gameReducer(GameState, Action);
