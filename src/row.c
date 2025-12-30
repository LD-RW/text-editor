#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"


int editorRowCxToRx(erow *row, int cx){
    int rx = 0;
    int j;
    for(j = 0; j < cx; ++j){
        if(row -> chars[j] == '\t'){
            rx += (TAB_STOP - 1) - (rx % TAB_STOP);
        }
        rx++;
    }
    return rx;
}


void editorUpdateRow(erow *row){
    int tabs = 0;
    int j = 0;
    for(j = 0; j < row -> size; ++j){
        if(row -> chars[j] == '\t') tabs++;
    }
    free(row -> render);
    row -> render = malloc(row -> size + tabs * (TAB_STOP - 1) + 1);
    int idx = 0;
    for(j = 0; j < row -> size; ++j){
        if(row -> chars[j] == '\t'){
            row -> render[idx++] = ' ';
            while(idx % TAB_STOP != 0) row -> render[idx++] = ' ';
        }
        else{
            row -> render[idx++] = row -> chars[j];
        }
    }
    row -> render[idx] = '\0';
    row -> rSize = idx;
}


void editorInsertRow(int at, char *s, size_t len){
    if(at < 0 || at > E.numRows) return;


    E.row = realloc(E.row, sizeof(erow) * (E.numRows + 1));
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numRows - at));
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';

    E.row[at].rSize = 0;
    E.row[at].render = NULL;
    editorUpdateRow(&E.row[at]);
    E.numRows++;
    E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c){
    if(at < 0 || at > row -> size) at = row -> size;
    row -> chars = realloc(row -> chars, row -> size + 2);
    memmove(&row -> chars[at + 1], &row -> chars[at], row -> size - at + 1);
    row -> size++;
    row -> chars[at] = c;
    editorUpdateRow(row);
    E.dirty++;
}


void editorRowDelChar(erow *row, int at){
    if(at < 0 || at >= row -> size) return;
    memmove(&row -> chars[at], &row -> chars[at + 1], row -> size - at);
    row -> size--;
    editorUpdateRow(row);
    E.dirty++;
}
void editorFreeRow(erow *row){
    free(row -> render);
    free(row -> chars);
}

void editorDelRow(int at){
    if(at < 0 || at >= E.numRows) return;
    editorFreeRow(&E.row[at]);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numRows - at - 1));
    E.numRows--;
    E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len){
    row -> chars = realloc(row -> chars, row -> size + len + 1);
    memcpy(&row -> chars[row -> size], s, len);
    row -> size += len;
    row -> chars[row -> size] = '\0';
    editorUpdateRow(row);
    E.dirty++;
}

int editorRowRxToCx(erow *row, int rx){
    int curRx = 0;
    int cx;
    for(cx = 0; cx < row -> size; cx++){
        if(row -> chars[cx] == '\t'){
            curRx += (TAB_STOP - 1) + (curRx % TAB_STOP);
        }
        curRx++;
        if(curRx > rx){
            return cx;
        }

    }
    return cx;
}