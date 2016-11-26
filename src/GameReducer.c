/*
    All the core game logic is defined as a reducer function:
    It receives the previous gameState, and an action to be performed
    It then returns the new gameState.
*/
#include "GameReducer.h"

Physics physicsReducer(Physics, Action);
GameState hostileReducer(GameState);
GameState projectileReducer(GameState, Action);

/*
    The main game reducer, as detailed above, it receives
    the previous gameState, parses which action is going
    to be performed, does all necessary modifications, and
    returns it as the new game state.
*/
GameState gameReducer(GameState lastState, Action action){
    lastState = hostileReducer(lastState);
    lastState.physics = physicsReducer(lastState.physics, action);
    // Collision checking
    // lower collision (floor)
    if(lastState.physics.height <= 0){
        lastState.gameEnded = 1;
    }
    lastState = projectileReducer(lastState, action);
    int i;
    for(i = 0; i < MAX_HOSTILES; i++){
        // player is in vertical pipe region
        if(lastState.hostile[i].active){
            if(lastState.hostile[i].type == TYPE_PIPE){
                if(lastState.physics.height >= lastState.hostile[i].pipe.gap[0] || lastState.physics.height <= lastState.hostile[i].pipe.gap[1]){
                    // player is in horizontal pipe region
                    if((lastState.physics.hPosition >= lastState.hostile[i].hPosition) && (lastState.physics.hPosition <= (lastState.hostile[i].hPosition + lastState.hostile[i].pipe.hWidth))){
                        // player collided with a pipe
                        lastState.gameEnded = 1;
                    }
                }
                // pipe wasnt scored yet
                if(!lastState.hostile[i].scored){
                    // player has passed the pipe
                    if(lastState.physics.hPosition > lastState.hostile[i].hPosition + lastState.hostile[i].pipe.hWidth){
                        lastState.points += 1;
                        lastState.hostile[i].scored = 1;
                    }
                }

                // pipe is not on the screen anymore
                if(lastState.physics.hPosition - PIPE_WIDTH > lastState.hostile[i].hPosition + lastState.hostile[i].pipe.hWidth){
                    // deactives it
                    lastState.hostile[i].active = 0;
                }
            }
        }
    }
    return lastState;
}

/*
    This specific reducer takes care of spawning new Pipes
    and managing all necessary modifications to them.
*/
GameState hostileReducer(GameState lastState){
    int i;
    if(!lastState.hostile[0].active){
        int nextEnemyType;
        if(lastState.spawnEnemies){

        } else {
            nextEnemyType = TYPE_PIPE;
        }
        Hostile newHostile;
        newHostile.active = 1;
        newHostile.scored = 0;
        newHostile.type = nextEnemyType;
        newHostile.hPosition = lastState.hostile[MAX_HOSTILES-1].hPosition + SPACE_BETWEEN_HOSTILES;
        if(nextEnemyType == TYPE_PIPE){
            Pipe newPipe;
            newPipe.gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
            newPipe.gap[1] = newPipe.gap[0] - PIPE_GAP;
            newPipe.hWidth = PIPE_WIDTH;
            newHostile.pipe = newPipe;
        }

        // pull all old hostiles to the front
        for(i = 1; i < MAX_HOSTILES; i++){
            lastState.hostile[i-1] = lastState.hostile[i];
        }

        // puts it at the end of the line
        lastState.hostile[MAX_HOSTILES-1] = newHostile;
    }
    return lastState;
}

/*
    The projectiles reducer is responsible for applying
    physics and collision detection for the projectiles
    in expert mode.
*/
GameState projectileReducer(GameState lastState, Action action){
    int i, j;
    float t = lastState.physics.elapsedTime;
    // Makes the projectiles progress in the screen
    for(i = 0; i < MAX_PROJECTILES; i++){
        if(lastState.projectile[i].active){
            lastState.projectile[i].hPosition += lastState.projectile[i].hVelocity * t;
            if((lastState.projectile[i].hPosition - lastState.physics.hPosition) > (MAX_WIDTH - 1)){
                lastState.projectile[i].active = 0;
            }
            for(j = 0; j < MAX_HOSTILES; j++){
                if(lastState.hostile[j].active){
                    // is projectile in pipes region?
                    if(lastState.projectile[i].height >= lastState.hostile[j].pipe.gap[0] || lastState.projectile[i].height <= lastState.hostile[j].pipe.gap[1]){
                        if((lastState.projectile[i].hPosition >= lastState.hostile[j].hPosition) && (lastState.projectile[i].hPosition <= (lastState.hostile[j].hPosition + lastState.hostile[j].pipe.hWidth))){
                            // projectile collided with a pipe
                            lastState.projectile[i].active = 0;
                        }
                    }
                }
            }
        }
    }
    if(action.type == ACTION_SHOOT){
        int created = 0;
        i = 0;
        while(!created && i < MAX_PROJECTILES){
            if(!lastState.projectile[i].active){
                lastState.projectile[i].active = 1;
                lastState.projectile[i].hPosition = lastState.physics.hPosition + PIPE_WIDTH;
                lastState.projectile[i].hVelocity = action.params[0];
                lastState.projectile[i].height = lastState.physics.height;
                created = 1;
            }
            i++;
        }
    }
    return lastState;
}

/*
    The physics reducer is reponsible to apply the game's
    physics to the current state, acording to the performed
    actions.
*/
Physics physicsReducer(Physics lastState, Action action){
    float h = lastState.height,
        v = lastState.velocity,
        a = lastState.gravity,
        t = lastState.elapsedTime;
    switch(action.type){
        case ACTION_FLAP:
            // Sets v to flap velocity
            v = action.params[0];
        break;
    }
    // h = h + vt + (at^2)/2
    lastState.height = h + (v*t) + ((a*t*t)/2);
    // v = v + at
    lastState.velocity = v + (a*t);
    // Collisions to the boundaries should reset the speed
    if(lastState.height <= 0){
        lastState.velocity = 0;
        lastState.height = 0;
    } else if(lastState.height >= lastState.maxHeight){
        lastState.velocity = 0;
        lastState.height = lastState.maxHeight;
    }

    // Horizontal movement
    lastState.hPosition += lastState.hVelocity * t;
    return lastState;
}
