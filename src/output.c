#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

/*** append buffer ***/
struct abuf {
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len){
    char *new = realloc(ab->b, ab->len + len);
    if(new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void abFree(struct abuf *ab){
    free(ab->b);
}

/*** output operations ***/
void editorScroll(){
    if(E.cy < E.rowOff){
        E.rowOff = E.cy;
    }
    if(E.cy >= E.rowOff + E.screenRows){
        E.rowOff = E.cy - E.screenRows + 1;
    }
    if(E.cx < E.colOff){
        E.colOff = E.cx;
    }
    if(E.cx >= E.colOff + E.screenCols){
        E.colOff = E.cx - E.screenCols + 1;
    }
}

static void editorDrawRows(struct abuf *ab){
    int y;
    for(y = 0; y < E.screenRows; ++y){
        int fileRow = y + E.rowOff;
        if(fileRow >= E.numRows) {
            if(E.numRows == 0 && y == E.screenRows / 3){
                char welcome[80];
                int welcomeLen = snprintf(welcome, sizeof(welcome), "B-textEditor --Version %s", B_TEXTEDITOR_VERSION);
                if(welcomeLen > E.screenCols) welcomeLen = E.screenCols;
                int padding = (E.screenCols - welcomeLen) / 2;
                if(padding){
                    abAppend(ab, "~", 1);
                    padding--;
                }
                while(padding--) abAppend(ab, " ", 1);
                abAppend(ab, welcome, welcomeLen);
            } else {
                abAppend(ab, "~", 1);
            }
        } else {
            int len = E.row[fileRow].rSize - E.colOff;
            if(len < 0) len = 0;
            if(len > E.screenCols) len = E.screenCols;
            abAppend(ab, &E.row[fileRow].render[E.colOff], len);
        }

        abAppend(ab, "\x1b[K", 3);
        if(y < E.screenRows - 1){
            abAppend(ab, "\r\n", 2);
        }
    }
}

void editorRefreshScreen(){
    editorScroll();
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);
    
    editorDrawRows(&ab);
    
    char buf[32];
    /* Fix: Logic to update cursor position relative to screen, not file */
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowOff) + 1, (E.cx - E.colOff) + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}