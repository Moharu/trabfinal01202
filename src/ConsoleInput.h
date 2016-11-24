// Return and ASCII codes definition for the ConsoleInput.c file,
// also its function's prototypes

// return codes
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_SPACE 3
#define KEY_RETURN 4
#define KEY_UNDEFINED 0

// ascii codes
#define ASCII_RETURN 10
#define ASCII_SPACEBAR 32
#define ASCII_ARROWS1 27
#define ASCII_ARROWS2 91
#define ASCII_ARROWUP 65
#define ASCII_ARROWDOWN 66

// function prototypes
int pollChar();
int getKeyPress();
void inputStartup();
