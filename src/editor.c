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