/*
    Flappy bird game!
    By João Vítor Bianchi Teló
    INF01202 - Algoritmos e Programação
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "ConsoleInput.h"
#include "GameConstants.h"
#include "ScreenBuilder.h"

int menuScreen(Screen*);
int normalGame(Screen*);
int expertGame();

int main(){
    int gameMode, gameIsRunning = 1, shouldStop = 0;
    inputStartup();
    outputStartup();
    srand(time(NULL));
    Screen screen;

    while(gameIsRunning){
        // Initializes on the menu screen
        // the return value is the selected game mode
        gameMode = menuScreen(&screen);

        if(gameMode == MODE_NORMAL){
            normalGame(&screen);
        }

        printMessage("You lose! Press anything to restart", 11, 16);
        int shouldStart = 0;
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

/*  Receives the screen struct where the information is rendered
    Returns the option selected by the user
 */
int menuScreen(Screen *screen){
    int i, selectedOption = 1, shouldStart = 0;
    // menu screen default game state
    GameState state;
    state.points = 0;
    state.physics.maxHeight = MAX_HEIGHT;
    state.physics.height = state.physics.maxHeight/2;
    // Initializes the pipes array with no pipes
    for(i = 0; i < MAX_PIPES; i++){
        state.pipe[i].active = 0;
        state.pipe[i].scored = 0;
        state.pipe[i].hPosition = 0;
        state.pipe[i].hWidth = 0;
        state.pipe[i].gap[0] = 0;
        state.pipe[i].gap[1] = 0;
    }

    *screen = buildScreenFromState(state);
    renderScreen(*screen);
    while(!shouldStart){
        printMessage("Flappy bird!", 9, 25);
        printMessage("Normal Game", 12, 26);
        printMessage("Expert Game", 13, 26);
        printMessage("*", 11 + selectedOption, 24);
        printMessage(" ", 12 + (selectedOption % 2), 24);
        int pressedKey = getKeyPress();
        switch(pressedKey){
            case KEY_RETURN:
                shouldStart = 1;
            break;
            case KEY_UP:
            case KEY_DOWN:
                if(selectedOption == MODE_NORMAL){
                    selectedOption = MODE_EXPERT;
                } else {
                    selectedOption = MODE_NORMAL;
                }
            break;
        }
        usleep(1000000/TIME_CONSTANT);
    }
    return selectedOption;
}

/* Default normal game mode, receives the screen struct where the game
   will be rendered
   Returns the player score after the game is ended.
*/
int normalGame(Screen *screen){
    int i, j, shouldStop = 0;
    // Game state initialization
    GameState state;
    state.gameEnded = 0;
    state.points = 0;
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
        state.pipe[i].scored = 0;
        state.pipe[i].hPosition = MAX_WIDTH + SPACE_BETWEEN_PIPES * i;
        state.pipe[i].hWidth = PIPE_WIDTH;
        state.pipe[i].gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
        state.pipe[i].gap[1] = state.pipe[i].gap[0] - PIPE_GAP;
    }

    Action act;
    act.type = ACTION_NONE;
    act.type = ACTION_FLAP;
    act.params[0] = FLAP_VELOCITY;
    state = gameReducer(state, act);

    while(!shouldStop){
        // Check if game is still running
        if(state.gameEnded){
            shouldStop = 1;
        }

        // User interface
        *screen = buildScreenFromState(state);
        renderScreen(*screen);

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
}


/* Expert game mode, receives the screen struct where the game
   will be rendered
   Returns the player score after the game is ended.
*/
int expertGame(){

}
