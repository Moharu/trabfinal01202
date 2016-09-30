#include "ScreenBuilder.h"
#include <stdio.h>
#include <stdlib.h>
#include "ConsoleOutput.h"

void loadTemplate(){
    FILE *template;
    template = fopen("assets/defaultTemplate.txt", "r");
    if(template == NULL){
        printf("Default template file not found.");
        exit(1);
    }
    int x=0, y=0;
    int c = fgetc(template);
    do {
        printf("%c", c);
        c = fgetc(template);
    } while(!feof(template));

    fclose(template);
}

void loadMenu(){

}
