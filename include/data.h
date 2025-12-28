#ifndef DATA_H
#define DATA_H

#include <termios.h>
#include<common.h>

typedef struct erow {
    int size;
    int rSize;
    char *chars;
    char *render;
} erow;

struct editorConfig {
    int cx, cy;
    int rx;
    int rowOff;
    int colOff;
    int screenRows;
    int screenCols;
    int numRows;
    char statusMsg[80];
    time_t statusMsgTime;
    erow *row;
    int dirty;
    char *fileName;
    struct termios orig_termios;
};

/* External declaration so all files share the same 'E' */
extern struct editorConfig E;

#endif