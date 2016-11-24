/*
    This file provides an interface with the console user input
*/
#include "ConsoleInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static struct termios saved_conf;

/*
    Polls the input buffer for the next character, returning it as an integer
*/
int pollChar() {
    int c;
    int read_ret = read(0, &c, 1);
    if (read_ret == 1)
        return c;
    else
        return -1;
}

/*
    Returns the currently pressed key as an integer constant defined
    as one of the return codes in the header file.
*/
int getKeyPress(){
    int c = pollChar();
    int returnCode = KEY_UNDEFINED;
    char pressedKey = 0;
    if(c != -1){
        pressedKey = (char) c;
        switch(pressedKey){
            // return key
            case ASCII_RETURN:
            returnCode = KEY_RETURN;
            break;
            // space bar
            case ASCII_SPACEBAR:
            returnCode = KEY_SPACE;
            break;
            // arrow keys
            case ASCII_ARROWS1:
                pressedKey = (char) pollChar();
                if(pressedKey == ASCII_ARROWS2){
                    pressedKey = (char) pollChar();
                    if(pressedKey == ASCII_ARROWUP){
                        returnCode = KEY_UP;
                    } else if(pressedKey == ASCII_ARROWDOWN){
                        returnCode = KEY_DOWN;
                    }
                }
            break;
        }
        while(c != -1) {
            c = pollChar();
        }
    }

    return returnCode;
}

/*
    This function is used as a callback at the programs exit,
    so it resets the standard input to its default state.
*/
void reset_stdin() {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_conf);
}

/*
    Provides the necessary startup to the standard input,
    such as defining the necessary callbacks and setting the
    necessary flags to have non-blocking input operations
*/
void inputStartup(){
    // Input startup
    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "Not running in a terminal\n");
        exit(1);
    }
    tcgetattr(STDIN_FILENO, &saved_conf);
    atexit(reset_stdin);
    struct termios conf;
    tcgetattr(STDIN_FILENO, &conf);
    conf.c_lflag &= ~(ECHO | ICANON);
    conf.c_cc[VMIN] = 0;
    conf.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conf) == -1) {
        fprintf(stderr, "tcsetattr failed\n");
        exit(1);
    }
}
