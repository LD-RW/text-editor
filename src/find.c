#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void editorFindCallBack(char *query, int key){
    static int lasMatch = -1;
    static int direction = 1;

    if(key == '\r' || key == '\x1b'){
        lasMatch = -1;
        direction = 1;
        return;
    }

    else if(key == ARROW_DOWN || key == ARROW_RIGHT){
        direction = 1;
    }
    else if(key == ARROW_LEFT || key == ARROW_UP){
        direction = -1;
    }
    else{
        lasMatch = -1;
        direction = 1;
    }

    if(lasMatch == -1) direction = 1;
    int current = lasMatch;

    int i;
    for(i = 0; i < E.numRows; ++i){
        current += direction;
        if(current == -1) current = E.numRows - 1;
        else if(current == E.numRows) current = 0;

        erow *row = &E.row[current];
        char *match = strstr(row -> render, query);
        if(match){
            lasMatch = current;
            E.cy = current;
            E.cx = editorRowRxToCx(row, match - row -> render);
            E.rowOff = E.numRows;
            break;
        }
    }
}


void editorFind(){
    int prevCx = E.cx;
    int prevCy = E.cy;
    int prevColOff = E.colOff;
    int prevRowOff = E.rowOff;
    char *query = editorPrompt("Search: %s (Use ESC/Arrows/Enter)",
                             editorFindCallBack);
    if(query == NULL) return;

    if(query){
        free(query);
    }
    else {
        E.cx = prevCx;
        E.cy = prevCy;
        E.colOff = prevColOff;
        E.rowOff = prevRowOff;
    }
}