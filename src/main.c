#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void initEditor(){
    E.cx = 0;
    E.cy = 0;
    E.rowOff = 0;
    E.colOff = 0;
    E.rx = 0;
    E.numRows = 0;
    E.row = NULL;
    E.fileName = NULL;
    E.statusMsg[0] = '\0';
    E.statusMsgTime = 0;
    if(getWindowSize(&E.screenRows, &E.screenCols) == -1){
        die("getWindowSize");
    }
    E.screenRows -= 2;
    
}

int main(int argc, char *argv[]){
    enableRawMode();
    initEditor();
    if(argc >= 2){
        editorOpen(argv[1]);
    }

    editorSetStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit");

    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}