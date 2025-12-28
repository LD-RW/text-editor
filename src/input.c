#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void editorMoveCursor(int key){
    erow *row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];


    switch (key) {
    case ARROW_LEFT:
        if(E.cx != 0){
            E.cx--;
        } 
        else if(E.cy > 0){
            E.cy--;
            E.cx = E.row[E.cy].size;
        }
        break;
    case ARROW_RIGHT:
        if(row && E.cx < row -> size){
            E.cx++;
        }
        else if(row && E.cx == row -> size){
            E.cy++;
            E.cx = 0;
        }
        break;
    case ARROW_UP:
        if(E.cy > 0)
            E.cy--;
        break;
    case ARROW_DOWN:
        if(E.cy < E.numRows)
            E.cy++;
        break;
    }
    row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];
    int rowLen = row ? row -> size : 0;
    if(E.cx > rowLen){
        E.cx = rowLen;
    }
}

void editorProcessKeypress(){

    static int quitTimes = B_QUIT_TIMES;

    int c = editorReadKey();
    switch(c) {
        case '\r':
            /* TO DO */
            break;


        case CTRL_KEY('q'):
            if(E.dirty && quitTimes > 0){
                editorSetStatusMessage("WARNING!!! File has unsaved changes. "
                "Press Ctrl-Q %d more times to quit.", quitTimes);
                quitTimes--;
                return;
            }


            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        case HOME_KEY:
            E.cx = 0;
            break;
        case END_KEY:
            if(E.cy < E.numRows){
                E.cx = E.row[E.cy].size;
            }
            break;

            case BACKSPACE:
            case CTRL_KEY('h'):
            case DEL_KEY:
                /* To Do */
                break;


        case PAGE_UP:
        case PAGE_DOWN:
            {
                if(c == PAGE_UP){
                    E.cy = E.rowOff;
                }
                else if(c == PAGE_DOWN){
                    E.cy = E.rowOff + E.screenRows - 1;
                    if(E.cy > E.numRows) E.cy = E.numRows;
                }



                int times = E.screenRows;
                while(times--){
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }   
            }
            break;
        case CTRL_KEY('s'):
            editorSave();
            break;
        case ARROW_UP:
        case ARROW_LEFT:
        case ARROW_DOWN:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;

        case CTRL_KEY('l'):
        case '\x1b':
            break;
        
        default:
            editorInsertChar(c);
            break;
    }
    quitTimes = B_QUIT_TIMES;
}