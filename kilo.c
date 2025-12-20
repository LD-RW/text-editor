// includes
#include<stdlib.h>
#include <unistd.h>
#include<termios.h>
#include<ctype.h>
#include<stdio.h>
#include<errno.h>

// data
struct termios orig_termious;

// terminal
void die(const char *s){
    perror(s);
    exit(1);
}

void disableRowMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termious) == -1) die("tcsetattr");
}

void enableRowMode(){
    if(tcgetattr(STDIN_FILENO, &orig_termious) == -1) die("tcgetattr");
    atexit(disableRowMode);

    struct termios raw = orig_termious;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termious) == -1) die("tcsetattr");
    
}

// main
int main(){
    enableRowMode();
    while(true){
        char c = '\0';
        if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if(iscntrl(c)){
            printf("%d\r\n", c);
        }
        else{
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == 'q') break;

    }
    return 0;
}