#include "ConsoleInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static struct termios saved_conf;

int pollChar() {
    int c;
    int read_ret = read(0, &c, 1);
    if (read_ret == 1)
        return c;
    else
        return -1;
}

void reset_stdin() {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_conf);
}

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
