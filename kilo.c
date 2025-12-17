#include<stdlib.h>
#include <unistd.h>
#include<termios.h>

struct termios orig_termious;

void disableRowMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termious);
}

void enableRowMode(){
    tcgetattr(STDIN_FILENO, &orig_termious);
    atexit(disableRowMode);

    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    
}

int main(){
    enableRowMode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}