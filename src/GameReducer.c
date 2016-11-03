#include "GameReducer.h"

Physics physicsReducer(Physics, Action);

GameState gameReducer(GameState lastState, Action action){
    lastState.physics = physicsReducer(lastState.physics, action);
    // Collision
    // lower collision
    if(lastState.physics.height <= 0){
        lastState.gameEnded = 1;
    }
    // is in vertical pipe region
    if(lastState.physics.height >= lastState.pipe.gap[0] || lastState.physics.height <= lastState.pipe.gap[1]){
        // is in horizontal pipe region
        if((lastState.physics.hPosition >= lastState.pipe.hPosition) && (lastState.physics.hPosition <= (lastState.pipe.hPosition + lastState.pipe.hWidth))){
            lastState.gameEnded = 1;
        }
    }
    return lastState;
}

GameState pipeReducer(GameState lastState, Pipe newPipe){
    lastState.pipe = newPipe;
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
