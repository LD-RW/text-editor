#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void editorInsertChar(int c){
    if(E.cy == E.numRows){
        editorAppendRow("", 0);
    }
    editorRowInsertChar(&E.row[E.cy], E.cx, c);
    E.cx++;
}

void editorDelChar(){
    if(E.cy == E.numRows) return;
    if(E.cx == 0 && E.cy == 0) return;
    erow *row = &E.row[E.cy];
    if(E.cx > 0){
        editorRowDelChar(row, E.cx - 1);
        E.cx--;
    }
    else{
        E.cx = E.row[E.cy - 1].size;
        editorRowAppendString(&E.row[E.cy - 1], row -> chars, row -> size);
        editorDelRow(E.cy);
        E.cy--;
    }
}