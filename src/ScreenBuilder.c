/*
    The ScreenBuilder is a translator from the
    gameState to the Screen structure to be rendered
*/
#include "ScreenBuilder.h"
#include "GameConstants.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
    Receives the current gameState, returning
    the Screen structure with all the game information
*/
Screen buildScreenFromState(GameState state) {
    Screen screen;
    int i,j;
    // clears the screen
    for(i = 0; i < SCREEN_HEIGHT; i++){
        for(j = 0; j < SCREEN_WIDTH; j++){
            screen.content[i][j] = ' ';
        }
    }
    // drawing the pipes
    for(i = 0; i < MAX_HOSTILES; i++){
        if(state.hostile[i].active){
            float widthRatio = SCREEN_WIDTH / MAX_WIDTH;
            int startRelativePosition = round((state.hostile[i].hPosition - state.physics.hPosition + PIPE_WIDTH) * widthRatio);
            if(state.hostile[i].type == TYPE_PIPE){
                int endRelativePosition = round((startRelativePosition + state.hostile[i].pipe.hWidth) * widthRatio);
                if(startRelativePosition >= - PIPE_WIDTH && startRelativePosition < 60){
                    // Draw the upper pipe
                    for(j = SCREEN_HEIGHT - 1; j >= state.hostile[i].pipe.gap[0]; j--){
                        if(startRelativePosition > 0){
                            screen.content[j][startRelativePosition] = '|';
                        }
                        if(endRelativePosition < SCREEN_WIDTH){
                            screen.content[j][endRelativePosition] = '|';
                        }
                    }
                    // Draw the lower pipe
                    for(j = state.hostile[i].pipe.gap[1] - 1; j >= 0; j--){
                        if(startRelativePosition > 0){
                            screen.content[j][startRelativePosition] = '|';
                        }
                        if(endRelativePosition < SCREEN_WIDTH){
                            screen.content[j][endRelativePosition] = '|';
                        }
                    }
                    // Draw the pipes' ends
                    if(startRelativePosition < 0){
                        startRelativePosition = -1;
                    }
                    if(endRelativePosition >= SCREEN_WIDTH){
                        endRelativePosition = SCREEN_HEIGHT;
                    }
                    for(j = startRelativePosition + 1; j < endRelativePosition; j++){
                        screen.content[(int)state.hostile[i].pipe.gap[0]][j] = '_';
                        screen.content[(int)state.hostile[i].pipe.gap[1]][j] = '_';
                    }
                }
            } else {
                int endRelativePosition = round((startRelativePosition + state.hostile[i].enemy.hWidth) * widthRatio);
                if(startRelativePosition >= - PIPE_WIDTH && startRelativePosition < 60){
                    // Draw the enemy
                    for(j = state.hostile[i].enemy.height - 1; j >= (state.hostile[i].enemy.height - ENEMY_HEIGHT); j--){
                        if(startRelativePosition > 0){
                            screen.content[j][startRelativePosition] = '/';
                        }
                        if(endRelativePosition < SCREEN_WIDTH){
                            screen.content[j][endRelativePosition] = '\\';
                        }
                    }
                    // Draw the enemies ends
                    if(startRelativePosition < 0){
                        startRelativePosition = -1;
                    }
                    if(endRelativePosition >= SCREEN_WIDTH){
                        endRelativePosition = SCREEN_HEIGHT;
                    }
                    for(j = startRelativePosition + 1; j < endRelativePosition; j++){
                        screen.content[(int)state.hostile[i].enemy.height - 1][j] = '/';
                        screen.content[(int)state.hostile[i].enemy.height - ENEMY_HEIGHT][j] = '\\';
                    }
                }
            }
        }
    }

    // Drawing projectiles
    for(i = 0; i < MAX_PROJECTILES; i++){
        // position pipe
        if(state.projectile[i].active){
            float widthRatio = SCREEN_WIDTH / MAX_WIDTH;
            int Ypos = round((state.projectile[i].height/state.physics.maxHeight)*(SCREEN_HEIGHT-1));
            int Xpos = round((state.projectile[i].hPosition - state.physics.hPosition) * widthRatio);
            screen.content[Ypos][Xpos] = '-';
        }
    }

    // position player
    // find current player position in screen
    int pos = round((state.physics.height/state.physics.maxHeight)*(SCREEN_HEIGHT*3 - 1));
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
    screen.content[posModule][PIPE_WIDTH] = p;

    // score
    char score[SCORE_MAXDIGITS + 7];
    sprintf(score, "Score: %d", state.points);
    int scoreLen = strlen(score);
    int scoreOffset = SCORE_OFFSET;

    for(i = scoreLen - 1; i >= 0; i--){
        screen.content[MAX_HEIGHT - 2][MAX_WIDTH - scoreOffset - 1] = score[i];
        scoreOffset += 1;
    }

    return screen;
}
