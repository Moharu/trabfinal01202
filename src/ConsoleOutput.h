/*
	Screen proportions, the Screen struct and the ConsoleOutput
	function prototypes
*/
#define SCREEN_WIDTH	60
#define SCREEN_HEIGHT	20

typedef struct screen {
	char content[SCREEN_HEIGHT][SCREEN_WIDTH];
} Screen;

void printMessage(char[], int, int);
void outputStartup();
void renderScreen(Screen);
