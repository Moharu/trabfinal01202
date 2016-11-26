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
#include <string.h>

#define SCORES_FILE "scores.dat"
#define waitNextFrame() usleep(1000000/TIME_CONSTANT)

int menuScreen(Screen*);
void normalGame(Screen*);
void expertGame();
void highscores(int, int);

int main(){
    int gameMode, gameIsRunning = 1;
    inputStartup();
    outputStartup();
    srand(time(NULL));
    Screen screen;

    while(gameIsRunning){
        // Initializes on the menu screen
        // the return value is the selected game mode
        gameMode = menuScreen(&screen);

        switch(gameMode){
            case MODE_NORMAL:
                normalGame(&screen);
            break;
            case MODE_EXPERT:
                expertGame(&screen);
            break;
            case MODE_EXIT:
                gameIsRunning = 0;
            break;
        }
    }
    return 0;
}

/*  Receives the screen struct where the information is rendered
    Returns the option selected by the user
 */
int menuScreen(Screen *screen){
    int i, selectedOption = MODE_NORMAL, shouldStart = 0;
    // menu screen default game state
    GameState state;
    state.points = 0;
    state.spawnEnemies = 0;
    state.physics.maxHeight = MAX_HEIGHT;
    state.physics.height = state.physics.maxHeight/2;
    // Initializes the pipes array with no pipes
    for(i = 0; i < MAX_HOSTILES; i++){
        state.hostile[i].active = 0;
    }
    // Initialize the projectiles array (disabled in normal mode)
    for(i = 0; i < MAX_PROJECTILES; i++){
        state.projectile[i].active = 0;
    }

    *screen = buildScreenFromState(state);
    renderScreen(*screen);
    while(!shouldStart){
        printMessage("Flappy bird!", 9, 25);
        printMessage("   Normal Game", 12, 23);
        printMessage("   Expert Game", 13, 23);
        printMessage("   Exit", 14, 23);
        printMessage("->", 11 + selectedOption, 23);
        int pressedKey = getKeyPress();
        switch(pressedKey){
            case KEY_RETURN:
                shouldStart = 1;
            break;
            case KEY_UP:
                selectedOption -= 1;
            break;
            case KEY_DOWN:
                selectedOption += 1;
            break;
        }
        if(selectedOption < MODE_NORMAL){
            selectedOption = MODE_EXIT;
        } else if(selectedOption > MODE_EXIT){
            selectedOption = MODE_NORMAL;
        }
        waitNextFrame();
    }
    return selectedOption;
}

/* Default normal game mode, receives the screen struct where the game
   will be rendered
*/
void normalGame(Screen *screen){
    int i, j, shouldStop = 0;
    // Game state initialization
    GameState state;
    state.gameEnded = 0;
    state.points = 0;
    state.spawnEnemies = 0;
    state.physics.maxHeight = MAX_HEIGHT;
    state.physics.gravity = GRAVITY_ACCEL;
    state.physics.velocity = 0;
    state.physics.elapsedTime = 1.0/TIME_CONSTANT;
    state.physics.height = state.physics.maxHeight/2;
    state.physics.hPosition = 0;
    state.physics.hVelocity = SCROLLING_SPEED;
    // Initializes the hostiles array with starting pipes
    for(i = 0; i < MAX_HOSTILES; i++){
        state.hostile[i].active = 1;
        state.hostile[i].scored = 0;
        state.hostile[i].type = TYPE_PIPE;
        state.hostile[i].hPosition = MAX_WIDTH + SPACE_BETWEEN_HOSTILES * i;
        state.hostile[i].pipe.hWidth = PIPE_WIDTH;
        state.hostile[i].pipe.gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
        state.hostile[i].pipe.gap[1] = state.hostile[i].pipe.gap[0] - PIPE_GAP;
    }
    // Initialize the projectiles array (disabled in normal mode)
    for(i = 0; i < MAX_PROJECTILES; i++){
        state.projectile[i].active = 0;
    }

    Action act;
    act.type = ACTION_FLAP;
    act.params[0] = FLAP_VELOCITY;
    state = gameReducer(state, act);
    *screen = buildScreenFromState(state);
    renderScreen(*screen);

    // wait for input to start game
    printMessage("Press Space to jump!", 11, 16);
    while(getKeyPress() != KEY_SPACE){
        waitNextFrame();
    }

    while(!shouldStop){
        // Check if game is still running
        if(state.gameEnded){
            shouldStop = 1;
        }

        // User interface
        *screen = buildScreenFromState(state);
        renderScreen(*screen);

        // Action creator
        int pressedKey = getKeyPress();
        if(pressedKey == KEY_SPACE){
            act.type = ACTION_FLAP;
            act.params[0] = FLAP_VELOCITY;
        } else {
            act.type = ACTION_NONE;
        }

        state = gameReducer(state, act);

        // TIME_CONSTANT determines how many frames per second
        waitNextFrame();
    }
    // after game
    highscores(MODE_NORMAL, state.points);
}


/* Expert game mode, receives the screen struct where the game
   will be rendered
*/
void expertGame(Screen *screen){
    int i, j, shouldStop = 0;
    // Game state initialization
    GameState state;
    state.gameEnded = 0;
    state.points = 0;
    state.spawnEnemies = 1;
    state.physics.maxHeight = MAX_HEIGHT;
    state.physics.gravity = GRAVITY_ACCEL;
    state.physics.velocity = 0;
    state.physics.elapsedTime = 1.0/TIME_CONSTANT;
    state.physics.height = state.physics.maxHeight/2;
    state.physics.hPosition = 0;
    state.physics.hVelocity = SCROLLING_SPEED;
    // Initialize the projectiles array
    for(i = 0; i < MAX_PROJECTILES; i++){
        state.projectile[i].active = 0;
    }
    // Initializes the hostile array with initial hostiles
    for(i = 0; i < MAX_HOSTILES; i++){
        state.hostile[i].active = 1;
        state.hostile[i].scored = 0;
        int hostileType = (rand() % 2)? TYPE_PIPE : TYPE_ENEMY;
        state.hostile[i].type = hostileType;
        state.hostile[i].hPosition = i == 0 ? MAX_WIDTH : (state.hostile[i-1].hPosition + (rand() % SPACE_BETWEEN_HOSTILES + SPACE_BETWEEN_HOSTILES));
        if(hostileType == TYPE_PIPE){
            state.hostile[i].pipe.hWidth = PIPE_WIDTH;
            state.hostile[i].pipe.gap[0] = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
            state.hostile[i].pipe.gap[1] = state.hostile[i].pipe.gap[0] - PIPE_GAP;
        } else {
            state.hostile[i].enemy.hWidth = PIPE_WIDTH;
            state.hostile[i].enemy.height = (rand() % (MAX_HEIGHT - PIPE_GAP - 2 * PIPE_MARGIN)) + (PIPE_GAP + PIPE_MARGIN);
            state.hostile[i].enemy.velocity = 0;
        }
    }

    Action act;
    act.type = ACTION_FLAP;
    act.params[0] = FLAP_VELOCITY;
    state = gameReducer(state, act);
    *screen = buildScreenFromState(state);
    renderScreen(*screen);

    // wait for input to start game
    printMessage("Press Space to jump!", 11, 16);
    printMessage("Press Enter to shoot!", 12, 16);
    while(getKeyPress() != KEY_SPACE){
        waitNextFrame();
    }

    while(!shouldStop){
        // Check if game is still running
        if(state.gameEnded){
            shouldStop = 1;
        }

        // User interface
        *screen = buildScreenFromState(state);
        renderScreen(*screen);

        // Action creator
        int pressedKey = getKeyPress();
        if(pressedKey == KEY_SPACE){
            act.type = ACTION_FLAP;
            act.params[0] = FLAP_VELOCITY;
        } else if(pressedKey == KEY_RETURN){
            act.type = ACTION_SHOOT;
            act.params[0] = PROJECTILE_SPEED;
        } else {
            act.type = ACTION_NONE;
        }

        state = gameReducer(state, act);
        // TIME_CONSTANT determines how many frames per second
        waitNextFrame();
    }
    // after game
    highscores(MODE_EXPERT, state.points);
}

/*
    Prints out the highscores for the selected
    game mode, also ask you to include a new one
    if a highscore is beaten
*/
void highscores(int gameMode, int playerScore){
    struct Score {
        char name[4];
        int points;
    };
    struct Leaderboard {
        struct Score normal[3];
        struct Score expert[3];
    };

    FILE *scoreFile;
    struct Leaderboard leaderboard;
    int i, error = 0;
    scoreFile = fopen(SCORES_FILE, "rb");
    if(scoreFile == NULL){
        // If no leaderboard was previously found, creates a blank one
        scoreFile = fopen(SCORES_FILE, "wb");
        if(scoreFile != NULL){
            for(i = 0; i < 3; i++){
                strcpy(leaderboard.normal[i].name, "AAA");
                leaderboard.normal[i].points = 0;
                strcpy(leaderboard.expert[i].name, "AAA");
                leaderboard.expert[i].points = 0;
            }
            if(fwrite(&leaderboard, sizeof(struct Leaderboard), 1, scoreFile) != 1){
                error = 1;
            }
        } else {
            error = 1;
        }
    } else {
        if(fread(&leaderboard, sizeof(struct Leaderboard), 1, scoreFile) != 1){
            error = 1;
        }
    }
    fclose(scoreFile);
    if(error){
        printMessage("Error loading Leaderboard,", 11, 16);
    } else {
        int scorePosition = 0;
        char *name;
        if(gameMode == MODE_NORMAL){
            if(playerScore > leaderboard.normal[0].points){
                scorePosition = 1;
            } else if(playerScore > leaderboard.normal[1].points){
                scorePosition = 2;
            } else if(playerScore > leaderboard.normal[2].points){
                scorePosition = 3;
            }
            if(scorePosition){
                for(i = 2; i >= scorePosition; i--){
                    leaderboard.normal[i].points = leaderboard.normal[i-1].points;
                }
                leaderboard.normal[(scorePosition - 1)].points = playerScore;
                strcpy(leaderboard.normal[(scorePosition - 1)].name, "AAA");
                name = leaderboard.normal[(scorePosition - 1)].name;
            }
        } else {
            if(playerScore > leaderboard.expert[0].points){
                scorePosition = 1;
            } else if(playerScore > leaderboard.expert[1].points){
                scorePosition = 2;
            } else if(playerScore > leaderboard.expert[2].points){
                scorePosition = 3;
            }
            if(scorePosition){
                for(i = 2; i >= scorePosition; i--){
                    leaderboard.expert[i].points = leaderboard.expert[i-1].points;
                }
                leaderboard.expert[(scorePosition - 1)].points = playerScore;
                strcpy(leaderboard.expert[(scorePosition - 1)].name, "AAA");
                name = leaderboard.expert[(scorePosition - 1)].name;
            }
        }
        if(scorePosition){
            clearScreen();
            printMessage("Game Over!", 7, 27);
            printMessage("New Highscore!", 7, 25);
            char displayString[15];
            int insertingName = 1, currentPosition = 0;
            sprintf(displayString, "Your score: %03d", playerScore);
            printMessage(displayString, 11, 24);
            while(insertingName){
                sprintf(displayString, "Enter your name: %s", name);
                printMessage("   ", 12, 39);
                printMessage("   ", 14, 39);
                printMessage("-", 12, 39 + currentPosition);
                printMessage("+", 14, 39 + currentPosition);
                printMessage(displayString, 13, 22);
                int pressedKey = getKeyPress();
                switch(pressedKey){
                    case KEY_UP:
                    name[currentPosition] -= 1;
                    break;
                    case KEY_DOWN:
                    name[currentPosition] += 1;
                    break;
                    case KEY_RETURN:
                    currentPosition += 1;
                    break;
                }
                if(currentPosition >= 3){
                    insertingName = 0;
                } else if(name[currentPosition] < 'A'){
                    name[currentPosition] = 'Z';
                } else if(name[currentPosition] > 'Z'){
                    name[currentPosition] = 'A';
                }
                waitNextFrame();
            }
            scoreFile = fopen(SCORES_FILE, "wb");
            if(scoreFile == NULL){
                error = 1;
            }
            if(fwrite(&leaderboard, sizeof(struct Leaderboard), 1, scoreFile) != 1){
                error = 1;
            }
            fclose(scoreFile);
        }
        // clears the screen
        clearScreen();
        char msg[15];
        printMessage("Game Over!", 7, 27);
        sprintf(msg, "Your score: %03d", playerScore);
        printMessage(msg, 8, 24);
        printMessage("Leaderboard", 10, 26);
        for(i = 0; i < 3; i++){
            struct Score score;
            if(gameMode == MODE_NORMAL){
                score = leaderboard.normal[i];
            } else if(gameMode == MODE_EXPERT){
                score = leaderboard.expert[i];
            }
            sprintf(msg, "%d. %03d - %s", i + 1, score.points, score.name);
            printMessage(msg, 12 + i, 25);
        }
    }
    printMessage("Press enter to continue.", 16, 19);
    while(getKeyPress() != KEY_RETURN){
        waitNextFrame();
    }
    return;
};
