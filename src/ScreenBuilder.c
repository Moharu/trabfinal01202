#include "ScreenBuilder.h"
#include "GameConstants.h"
#include <math.h>
#include <stdio.h>

Screen buildScreenFromState(GameState state) {
    Screen screen;
    int i,j;
    // clears the screen
    for(i = 0; i < SCREEN_HEIGHT; i++){
        for(j = 0; j < SCREEN_WIDTH; j++){
            screen.content[i][j] = ' ';
        }
    }

    // position pipe
    float widthRatio = SCREEN_WIDTH / MAX_WIDTH;
    int startRelativePosition = round((state.pipe.hPosition - state.physics.hPosition + PIPE_WIDTH) * widthRatio);
    int endRelativePosition = round((startRelativePosition + state.pipe.hWidth) * widthRatio);
    if(startRelativePosition >= - PIPE_WIDTH && startRelativePosition < 60){
        // Draw the upper pipe
        for(i = SCREEN_HEIGHT - 1; i >= state.pipe.gap[0]; i--){
            if(startRelativePosition > 0){
                screen.content[i][startRelativePosition] = '|';
            }
            if(endRelativePosition < SCREEN_WIDTH){
                screen.content[i][endRelativePosition] = '|';
            }
        }
        // Draw the lower pipe
        for(i = state.pipe.gap[1] - 1; i >= 0; i--){
            if(startRelativePosition > 0){
                screen.content[i][startRelativePosition] = '|';
            }
            if(endRelativePosition < SCREEN_WIDTH){
                screen.content[i][endRelativePosition] = '|';
            }   
        }
        // Draw the pipes' ends
        if(startRelativePosition < 0){
            startRelativePosition = -1;
        }
        if(endRelativePosition >= SCREEN_WIDTH){
            endRelativePosition = SCREEN_HEIGHT;
        }
        for(i = startRelativePosition + 1; i < endRelativePosition; i++){
            screen.content[(int)state.pipe.gap[0]][i] = '_';
            screen.content[(int)state.pipe.gap[1]][i] = '_';
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

    return screen;
}