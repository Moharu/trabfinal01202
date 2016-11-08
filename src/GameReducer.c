#include "GameReducer.h"

Physics physicsReducer(Physics, Action);

GameState gameReducer(GameState lastState, Action action){
    lastState = pipeReducer(lastState);
    lastState.physics = physicsReducer(lastState.physics, action);
    // Collision
    // lower collision
    if(lastState.physics.height <= 0){
        lastState.gameEnded = 1;
    }
    int i;
    for(i = 0; i < MAX_PIPES; i++){
        // is in vertical pipe region
        if(lastState.pipe[i].active){
            if(lastState.physics.height >= lastState.pipe[i].gap[0] || lastState.physics.height <= lastState.pipe[i].gap[1]){
                // is in horizontal pipe region
                if((lastState.physics.hPosition >= lastState.pipe[i].hPosition) && (lastState.physics.hPosition <= (lastState.pipe[i].hPosition + lastState.pipe[i].hWidth))){
                    lastState.gameEnded = 1;
                }
            }
            // has passed the pipe
            if(lastState.physics.hPosition > lastState.pipe[i].hPosition + lastState.pipe[i].hWidth){
                // lastState.pipe[i].active = 0;
            }

            // pipe left the screen
            if(lastState.physics.hPosition - PIPE_WIDTH > lastState.pipe[i].hPosition + lastState.pipe[i].hWidth){
                lastState.pipe[i].active = 0;
            }
        }
    }
    return lastState;
}

GameState pipeReducer(GameState lastState){
    int i;
    if(!lastState.pipe[0].active){
        // if first pipe is inactive, create a new one
        Pipe newPipe;
        newPipe = lastState.pipe[0];
        newPipe.active = 1;
        newPipe.gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
        newPipe.gap[1] = newPipe.gap[0] - PIPE_GAP;
        newPipe.hPosition = lastState.pipe[MAX_PIPES-1].hPosition + SPACE_BETWEEN_PIPES;

        // pull all old pipes to the front
        for(i = 1; i < MAX_PIPES; i++){
            lastState.pipe[i-1] = lastState.pipe[i];
        }

        // put it at the end of the line
        lastState.pipe[MAX_PIPES-1] = newPipe;
    }
    return lastState;
}

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
    // h = h + vt + at^2/2
    lastState.height = h + (v*t) + ((a*t*t)/2);
    // v = v + at
    lastState.velocity = v + (a*t);
    // Collision
    if(lastState.height <= 0){
        lastState.velocity = 0;
        lastState.height = 0;
    } else if(lastState.height >= lastState.maxHeight){
        lastState.velocity = 0;
        lastState.height = lastState.maxHeight;
    }

    // Horizontal movement
    lastState.hPosition += lastState.hVelocity*t;
    return lastState;
}
