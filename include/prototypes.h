#ifndef PROTOTYPES_H
#define PROTOTYPES_H
#include "common.h"
#include "data.h"

// struct abuf;
// terminal.c
void die(const char *s);
void disableRawMode();
void enableRawMode();
int editorReadKey();
int getWindowSize(int *rows, int *cols);
int getCursorPosition(int *rows, int *cols);

// row.c
void editorInsertRow(int at, char *s, size_t len);
void editorUpdateRow(erow *row);
int editorRowCxToRx(erow *row, int cx);
void editorRowInsertChar(erow *row, int at, int c);
void editorRowDelChar(erow *row, int at);
void editorFreeRow(erow *row);
void editorDelRow(int at);
void editorRowAppendString(erow *row, char *s, size_t len);
int editorRowRxToCx(erow *row, int rx);

// file_io.c
void editorOpen(char *fileName);
char *editorRowsToString(int *buflen);
void editorSave();
// output.c
void editorRefreshScreen();
void editorScroll();
void editorSetStatusMessage(const char *fmt, ...);
// input.c
void editorMoveCursor(int key);
void editorProcessKeypress();
char *editorPrompt(char *prompt, void (*callback)(char *, int));

// main.c
void initEditor();

// editor.c
void editorInsertChar(int c);
void editorDelChar();
void editorInsertNewline();

// find.c
void editorFind();

#endif