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
    E.rx = 0;
    if(E.cy < E.numRows){
        E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
    }
    if(E.cy < E.rowOff){
        E.rowOff = E.cy;
    }
    if(E.cy >= E.rowOff + E.screenRows){
        E.rowOff = E.cy - E.screenRows + 1;
    }
    if(E.rx < E.colOff){
        E.colOff = E.rx;
    }
    if(E.rx >= E.colOff + E.screenCols){
        E.colOff = E.rx - E.screenCols + 1;
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
       
            abAppend(ab, "\r\n", 2);
        }
    
}

void editorDrawStatusBar(struct abuf *ab){
    abAppend(ab, "\x1b[7m", 4);
    char status[80], rStatus[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d lines", 
        E.fileName ? E.fileName : "[No Name]", E.numRows);
    int rLen = snprintf(rStatus, sizeof(rStatus), "%d/%d", E.cy + 1, E.numRows);
    if(len > E.screenCols) len = E.screenCols;
    abAppend(ab, status, len);
    while(len < E.screenCols){
        if(E.screenCols - len == rLen){
            abAppend(ab, rStatus, rLen);
            break;
        }
        else{
            abAppend(ab, " ", 1);
            len++;
        }
    }
    abAppend(ab, "\x1b[m", 3);
    abAppend(ab, "\r\n", 2);

}



void editorRefreshScreen(){

    editorScroll();
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);
    
    editorDrawRows(&ab);
    editorDrawStatusBar(&ab);
    
    char buf[32];
    /* Fix: Logic to update cursor position relative to screen, not file */
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowOff) + 1, (E.rx - E.colOff) + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}


void editorSetStatusMessage(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(E.statusMsg, sizeof(E.statusMsg), fmt, ap);
    va_end(ap);
    E.statusMsgTime = time(NULL);
}