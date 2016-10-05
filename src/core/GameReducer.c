#include <string.h>

char *(gameReducer)(char *lastState, char *action, char *newState){
    char *state = newState;
    strcpy(state, lastState);
    strcat(state, action);
    return newState;
}
