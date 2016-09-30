/*
    Flappy bird!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ConsoleInput.h"
#include "ConsoleOutput.h"
#include "ScreenBuilder.h"

#define FPS 30

int main(){
    int shouldExit = 0;
    inputStartup();
    outputStartup();
    loadTemplate();

    while(!shouldExit){
        int c = pollChar();
        if (c != -1) {
            printf("Key: ");

            // le todos os bytes disponiveis no buffer de entrada
            while (c != -1) {
                printf("0x%2x ", c);
                c = pollChar();
            }

            printf("\n");
        }
        usleep(1000/FPS);
    }
    return 0;
}
