#define SCREEN_WIDTH	60
#define SCREEN_HEIGHT	20

typedef struct screen {
	char content[SCREEN_HEIGHT][SCREEN_WIDTH];
} Screen;

void outputStartup();
void renderScreen(Screen);
