/*
    Flappy bird!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "ConsoleInput.h"
#include "GameConstants.h"
#include "ScreenBuilder.h"

int main(){
    int shouldExit = 0;
    inputStartup();
    outputStartup();

    // Game state initialization
    GameState state;
    state.gameEnded = 0;    
    state.physics.maxHeight = MAX_HEIGHT;
    state.physics.gravity = GRAVITY_ACCEL;
    state.physics.velocity = 0;
    state.physics.elapsedTime = 1.0/TIME_CONSTANT;
    // state.physics.height = state.physics.maxHeight / 2;
    state.physics.height = 18;
    state.physics.hPosition = 0;
    state.physics.hVelocity = SCROLLING_SPEED;
    // Initializes the pipes array with no pipes
    state.pipe.hPosition = -11;
    state.pipe.hWidth = PIPE_WIDTH;
    state.pipe.gap[0] = 14;
    state.pipe.gap[1] = 10;

    Action act;
    act.type = ACTION_NONE;
    Screen screen;
    while(!shouldExit){
        int i, j;

        // Check if game is still running
        if(state.gameEnded){  
            shouldExit = 1;
        }

        // User interface
        screen = buildScreenFromState(state);

        renderScreen(screen);
        printf("x: %.2f, p: %.2f\n", state.physics.hPosition, state.pipe.hPosition);
        printf("h: %.2f, v: %.2f\n",state.physics.height, state.physics.velocity);

        // Action creator
        int flap = 0; // Should flap if any input is received
        int c = pollChar();
        if (c != -1) {
            flap = 1;
            while (c != -1) {
                c = pollChar();
            }
        }
        if(flap){
            act.type = ACTION_FLAP;
            act.params[0] = FLAP_VELOCITY;
        } else {
            act.type = ACTION_NONE;
        }
        // Pipe
        if(state.physics.hPosition - state.pipe.hPosition > 12){
            state.pipe.hPosition = round(state.physics.hPosition + 55);
        }

        state = gameReducer(state, act);

        // TIME_CONSTANT determines how many frames per second
        usleep(1000000/TIME_CONSTANT);
    }
    return 0;
}
