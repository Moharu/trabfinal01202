/*
    Flappy bird!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "ConsoleInput.h"
#include "ConsoleOutput.h"
#include "ScreenBuilder.h"
#include "GameReducer.h"
#include "GameConstants.h"

int main(){
    int shouldExit = 0;
    inputStartup();
    // outputStartup();
    // loadTemplate();
    GameState state;
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
    state.pipe.hWidth = 5;
    state.pipe.gap[0] = 14;
    state.pipe.gap[1] = 8;

    Action act;
    act.type = ACTION_NONE;
    char screen[20][60];
    while(!shouldExit){
        // render screen
        int pos = round((state.physics.height/state.physics.maxHeight)*60); 
        int i, j;
        //clear screen
        printf("\e[2J");
        for(i=0;i<20;i++){
            for(j=0;j<60;j++){
                screen[i][j] = ' ';
            }
        }
        printf("x: %.2f, p: %.2f\n", state.physics.hPosition, state.pipe.hPosition);
        printf("h: %.2f, v: %.2f\n",state.physics.height, state.physics.velocity);
        printf("============================================================\n");
        // position pipe
        int startRelativePosition = round(state.pipe.hPosition - state.physics.hPosition) + 5;
        int endRelativePosition = round(startRelativePosition + state.pipe.hWidth);
        if(startRelativePosition >= -5 && startRelativePosition < 60){
            for(i=19; i >= state.pipe.gap[0]; i--){
                if(startRelativePosition > 0){
                    screen[i][startRelativePosition] = '|';
                }
                if(endRelativePosition < 60){
                    screen[i][endRelativePosition] = '|';
                }
            }
            for(i=state.pipe.gap[1]-1; i >= 0; i--){
                if(startRelativePosition > 0){
                    screen[i][startRelativePosition] = '|';
                }
                if(endRelativePosition < 60){
                    screen[i][endRelativePosition] = '|';
                }   
            }
            if(startRelativePosition < 0){
                startRelativePosition = -1;
            }
            if(endRelativePosition >= 60){
                endRelativePosition = 59;
            }
            for(i = startRelativePosition + 1; i < endRelativePosition; i++){
                screen[(int)state.pipe.gap[0]][i] = '_';
                screen[(int)state.pipe.gap[1]][i] = '_';
            }
        }

        // position player
        int posModule = pos/3;
        char p;
        switch(pos%3){
            case 0:
                p='.';
            break;
            case 1:
                p='-';
            break;
            case 2:
                p='\'';
            break;
        }
        screen[posModule][5] = p;

        for(i = 19; i >= 0; i--){
            for(j = 0; j < 60; j++){
                printf("%c", screen[i][j]);
            }
            printf("\n");
        }
        printf("============================================================\n");
        // update state
        int flap = 0;
        int c = pollChar();
        if (c != -1) {
            flap = 1;
            // le todos os bytes disponiveis no buffer de entrada
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
        // collision
        // is in horizontal region
        if((state.physics.hPosition >= state.pipe.hPosition) && (state.physics.hPosition <= (state.pipe.hPosition + state.pipe.hWidth))){
            // is in vertical region
            if(state.physics.height >= state.pipe.gap[0] || state.physics.height <= state.pipe.gap[1]){
                shouldExit = 1;
            }
        }
        state = gameReducer(state, act);
        usleep(1000000/TIME_CONSTANT);
    }
    return 0;
}
