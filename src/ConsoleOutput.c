#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ConsoleOutput.h"

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

void drawBoundary(int length){
	int i;
	for(i = 0; i < length; i++){
		printf("=");
	}
	printf("\n");
}

void renderScreen(Screen screen){
	clearScreen();
	int i, j;
	drawBoundary(SCREEN_WIDTH);
	// draw game
	for(i = SCREEN_HEIGHT - 1; i >= 0; i--){
		for(j = 0; j < SCREEN_WIDTH; j++){
			printf("%c", screen.content[i][j]);
		}
		printf("\n");
	}
	drawBoundary(SCREEN_WIDTH);
}