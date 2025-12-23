#ifndef DATA_H
#define DATA_H

#include<termios.h>

typedef struct erow{
    int size;
    char *chars;
} erow;

struct editorConfig{
    int cx, cy;
    int screenRows;
    int screenCols;
    int numRows;
    erow *row;
    struct termios orig_termios;

};

extern struct editorConfig E;




#endif