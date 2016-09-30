#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define clearScreen() printf("\e[2J")
#define resetTerminalState() printf("\ec")
#define resetCursorPosition() printf("\e[H")
#define hideCursor() printf("\e[?25l")

void reset() {
    resetTerminalState();
    clearScreen();
    resetCursorPosition();
}

void outputStartup(){
    clearScreen();
    hideCursor();
    resetCursorPosition();
    atexit(reset);
}
