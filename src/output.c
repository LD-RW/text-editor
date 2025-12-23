#include "common.h"
#include "data.h"
#include "prototypes.h"
struct abuf{
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0};

void abAppend(struct abuf *ab, const char *s, int len){
    char *new = realloc(ab -> b, ab -> len + len);

    if(new == NULL) return;
    memcpy(&new[ab -> len], s, len);
    ab -> b = new;
    ab -> len += len;
}

void abFree(struct abuf *ab){
    free(ab -> b);
}

void editorDrawRows(struct abuf *ab){
    int y;
    for(y = 0; y < E.screenRows; ++y){
        if(y >= E.numRows) {

            if(y == E.screenRows / 3 && E.numRows == 0){
            char welcome[80];
            int welcomeLen = snprintf(welcome, sizeof(welcome), "B-textEditor --Version %s", B_TEXTEDITOR_VERSION);
            if(welcomeLen > E.screenCols){
                welcomeLen = E.screenCols;
            }
            int padding  = (E.screenCols - welcomeLen) / 2;
            if(padding){
                abAppend(ab, "~", 1);
                padding--;
            }
            while(padding--) abAppend(ab, " ", 1);
            abAppend(ab, welcome, welcomeLen);
            }
            else{
                abAppend(ab, "~", 1);
            }
        }

        else{
            int len = E.row[y].size;
            if(len > E.screenCols) len = E.screenCols;
            abAppend(ab, E.row[y].chars, len);
        }

        abAppend(ab, "\x1b[K", 3);
        if(y < E.screenRows - 1){
            abAppend(ab, "\r\n", 2);
        }
    }
}

void editorRefreshScreen(){
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);
    
    editorDrawRows(&ab);
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}


