/*
    This file provides an easier interface with the console output
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ConsoleOutput.h"

// printf console macros
#define clearScreen() printf("\e[2J")
#define resetTerminalState() printf("\ec")
#define resetCursorPosition() printf("\e[H")
#define hideCursor() printf("\e[?25l")

/*
    Resets the terminal state
*/
void reset() {
    resetTerminalState();
    clearScreen();
    resetCursorPosition();
}

/*
    Provides the necessary output startup,
    such as clearing the screen, hiding the cursor
    and resetting its position
*/
void outputStartup(){
    clearScreen();
    hideCursor();
    resetCursorPosition();
    atexit(reset);
}

/*
    Draws the game playable boundaries, defined as two
    big '=' lines in the screen.
*/
void drawBoundary(int length){
	int i;
	for(i = 0; i < length; i++){
		printf("=");
	}
	printf("\n");
}

/*
    Prints the input msg at position x,y in the console
*/
void printMessage(char msg[], int x, int y){
    printf("\e[%d;%dH", x, y);
    puts(msg);
}

/*
    Receives an Screen structure, which contains all game
    information to be rendered cell by cell.
*/
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
