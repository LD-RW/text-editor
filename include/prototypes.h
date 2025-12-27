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
int getCursorPosition(int *rows, int *cols);

// row.c
void editorAppendRow(char *s, size_t len);

// file_io.c
void editorOpen(char *fileName);
// output.c
void editorDrawRows(struct abuf *ab);
void editorRefreshScreen();
void editorScroll();
// input.c
void editorMoveCursor(int key);
void editorProcessKeypress();

// main.c
void initEditor();

#endif