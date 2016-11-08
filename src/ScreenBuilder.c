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

    for(i = 0; i < MAX_PIPES; i++){
        // position pipe
        if(state.pipe[i].active){
            float widthRatio = SCREEN_WIDTH / MAX_WIDTH;
            int startRelativePosition = round((state.pipe[i].hPosition - state.physics.hPosition + PIPE_WIDTH) * widthRatio);
            int endRelativePosition = round((startRelativePosition + state.pipe[i].hWidth) * widthRatio);
            if(startRelativePosition >= - PIPE_WIDTH && startRelativePosition < 60){
                // Draw the upper pipe
                for(j = SCREEN_HEIGHT - 1; j >= state.pipe[i].gap[0]; j--){
                    if(startRelativePosition > 0){
                        screen.content[j][startRelativePosition] = '|';
                    }
                    if(endRelativePosition < SCREEN_WIDTH){
                        screen.content[j][endRelativePosition] = '|';
                    }
                }
                // Draw the lower pipe
                for(j = state.pipe[i].gap[1] - 1; j >= 0; j--){
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
                    screen.content[(int)state.pipe[i].gap[0]][j] = '_';
                    screen.content[(int)state.pipe[i].gap[1]][j] = '_';
                }
            }
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
