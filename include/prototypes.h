#ifndef PROTOTYPES_H
#define PROTOTYPES_H
#include "common.h"
#include "data.h"

// terminal.c
void die(const char *s);
void disableRawMode();
void enableRawMode();
int editorReadKey();
int getWindowSize(int *rows, int *cols);

// row.c
void editorAppendRow(char *s, size_t len);

// file_io.c
void editorOpen(char *fileName);

// output.c
void editorRefreshScreen();

// input.c
void editorProcessKeypress();

// main.c
void initEditor();

#endif