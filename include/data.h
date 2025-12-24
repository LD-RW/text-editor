#ifndef DATA_H
#define DATA_H

#include<termios.h>

/* 
* A container that holds the data for each line. It contains the starting
  location of the line in memory and the number of bytes in the line
  explicitly so the editor doesn't need to scan for the null terminator at the end of the line. 
* This allows us to use memcpy instead of strcpy(Which stops once it finds '\0'), so that if our
  line contains a null terminator in the middle (e.g., "Hello \0 World"), or if we forget to put
  a null terminator at the end of the line, we don't lose data or get stuck in an infinite loop
  of reading which will crash or program.
*/
typedef struct erow{
    int size;
    char *chars;
} erow;
/*
* The brain of our program, a global state that all the files can see and interact with
* [cx, cy]: Cursor's X and Y coordinates
* [sreenRows, screenCols]: The current width and hight of the terminal, so the program
  doesn't try to write out of the edge of the window
* numRows : The total number of rows in the current opened file
* *row : A pointer to the beginning of our array of lines
* orig_termois : The original terminal settings before we open the program, so we can set the 
  the terminal back to it's settings before the start of our program, because we changed how the
  terminal works when we entered the raw mode


*/
struct editorConfig{
    int cx, cy;
    int screenRows;
    int screenCols;
    int numRows;
    erow *row;
    struct termios orig_termios;

};
/*

* If you put struct editorConfig E; in both files, the Linker will crash,
  saying: "Error: Multiple definition of E.", and if you put it only in the 
  "main.c", the "editor.c" and all other files won't be able to see it
* The extern tells the C compiler that the variable exists, but it's defined in another file.
  We used it here so we can define our editorConfig struct in the "data.c" file but include
  this header in all files so all of them can see the same version of our struct
*/
extern struct editorConfig E;




#endif