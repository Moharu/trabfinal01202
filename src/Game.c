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
    state.maxHeight = MAX_HEIGHT;
    state.gravity = GRAVITY_ACCEL;
    state.velocity = 0;
    state.elapsedTime = 1.0/TIME_CONSTANT;
    // state.height = state.maxHeight / 2;
    state.height = 18;

    Action act;
    act.type = ACTION_NONE;
    while(!shouldExit){
        // render game
        int pos = round((state.height/state.maxHeight)*60); 
        printf("\e[2J");
        int i;
        printf("h: %.2f, v: %.2f\n",state.height, state.velocity);
        printf("============================================================\n");
        for(i=20; i>=0;i--){
            int posModule = pos/3;
            if(i==posModule){
                char c;
                switch(pos%3){
                    case 0:
                        c='.';
                    break;
                    case 1:
                        c='-';
                    break;
                    case 2:
                        c='\'';
                    break;
                }
                printf("\t%c\n",c);
            } else {
                printf("\n");
            }
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
        state = gameReducer(state, act);
        usleep(1000000/TIME_CONSTANT);
    }
    return 0;
}
