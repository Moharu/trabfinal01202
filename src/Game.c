/*
    Flappy bird!
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "ConsoleInput.h"
#include "GameConstants.h"
#include "ScreenBuilder.h"

int main(){
    int playing = 1, shouldStart = 0, shouldStop = 0, i, j;
    inputStartup();
    outputStartup();
    srand(time(NULL));

    while(playing){
        // Game state initialization
        GameState state;
        state.gameEnded = 0;
        state.physics.maxHeight = MAX_HEIGHT;
        state.physics.gravity = GRAVITY_ACCEL;
        state.physics.velocity = 0;
        state.physics.elapsedTime = 1.0/TIME_CONSTANT;
        state.physics.height = state.physics.maxHeight/2;
        state.physics.hPosition = 0;
        state.physics.hVelocity = SCROLLING_SPEED;
        // Initializes the pipes array with no pipes
        for(i = 0; i < MAX_PIPES; i++){
            state.pipe[i].active = 1;
            state.pipe[i].hPosition = MAX_WIDTH + SPACE_BETWEEN_PIPES * i;
            state.pipe[i].hWidth = PIPE_WIDTH;
            state.pipe[i].gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
            state.pipe[i].gap[1] = state.pipe[i].gap[0] - PIPE_GAP;
        }

        Action act;
        act.type = ACTION_NONE;
        Screen screen = buildScreenFromState(state);
        screen = putMessage(screen, "Flappy bird! Press to start");
        renderScreen(screen);

        while(!shouldStart){
            int c = pollChar();
            if(c != -1){
                shouldStart = 1;
                while(c != -1) {
                    c = pollChar();
                }
            }
        }

        act.type = ACTION_FLAP;
        act.params[0] = FLAP_VELOCITY;
        state = gameReducer(state, act);

        while(!shouldStop){
            // Check if game is still running
            if(state.gameEnded){
                shouldStop = 1;
            }

            // User interface
            screen = buildScreenFromState(state);
            renderScreen(screen);

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

            state = gameReducer(state, act);

            // TIME_CONSTANT determines how many frames per second
            usleep(1000000/TIME_CONSTANT);
        }
        screen = putMessage(screen, "You lose! Press anything to restart");
        renderScreen(screen);
        shouldStart = 0;
        while(!shouldStart){
            int c = pollChar();
            if(c != -1){
                shouldStart = 1;
                while(c != -1) {
                    c = pollChar();
                }
            }
        }
        shouldStart = 0;
        shouldStop = 0;
    }
    return 0;
}
