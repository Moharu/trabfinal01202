/*
    All the core game logic is defined as a reducer function:
    It receives the previous gameState, and an action to be performed
    It then returns the new gameState.
*/
#include "GameReducer.h"

Physics physicsReducer(Physics, Action);
GameState pipeReducer(GameState lastState);

/*
    The main game reducer, as detailed above, it receives
    the previous gameState, parses which action is going
    to be performed, does all necessary modifications, and
    returns it as the new game state.
*/
GameState gameReducer(GameState lastState, Action action){
    lastState = pipeReducer(lastState);
    lastState.physics = physicsReducer(lastState.physics, action);
    // Collision checking
    // lower collision (floor)
    if(lastState.physics.height <= 0){
        lastState.gameEnded = 1;
    }
    int i;
    for(i = 0; i < MAX_PIPES; i++){
        // player is in vertical pipe region
        if(lastState.pipe[i].active){
            if(lastState.physics.height >= lastState.pipe[i].gap[0] || lastState.physics.height <= lastState.pipe[i].gap[1]){
                // player is in horizontal pipe region
                if((lastState.physics.hPosition >= lastState.pipe[i].hPosition) && (lastState.physics.hPosition <= (lastState.pipe[i].hPosition + lastState.pipe[i].hWidth))){
                    // player collided with a pipe
                    lastState.gameEnded = 1;
                }
            }
            // pipe wasnt scored yet
            if(!lastState.pipe[i].scored){
                // player has passed the pipe
                if(lastState.physics.hPosition > lastState.pipe[i].hPosition + lastState.pipe[i].hWidth){
                    lastState.points += 1;
                    lastState.pipe[i].scored = 1;
                }
            }

            // pipe is not on the screen anymore
            if(lastState.physics.hPosition - PIPE_WIDTH > lastState.pipe[i].hPosition + lastState.pipe[i].hWidth){
                // deactives it
                lastState.pipe[i].active = 0;
            }
        }
    }
    return lastState;
}

/*
    This specific reducer takes care of spawning new Pipes
    and managing all necessary modifications to them.
*/
GameState pipeReducer(GameState lastState){
    int i;
    if(!lastState.pipe[0].active){
        // if first pipe is inactive, create a new one
        Pipe newPipe;
        newPipe = lastState.pipe[0];
        newPipe.active = 1;
        newPipe.scored = 0;
        newPipe.gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
        newPipe.gap[1] = newPipe.gap[0] - PIPE_GAP;
        newPipe.hPosition = lastState.pipe[MAX_PIPES-1].hPosition + SPACE_BETWEEN_PIPES;

        // pull all old pipes to the front
        for(i = 1; i < MAX_PIPES; i++){
            lastState.pipe[i-1] = lastState.pipe[i];
        }

        // puts it at the end of the line
        lastState.pipe[MAX_PIPES-1] = newPipe;
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
