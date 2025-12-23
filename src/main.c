#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"



void initEditor(){
    E.cx = 0;
    E.cy = 0;
    E.numRows = 0;
    E.row = NULL;
    if(getWindowSize(&E.screenRows, &E.screenCols) == -1){
        die("getWindowSize");
    }
}

int main(int argc, char *argv[]){
    enableRawMode();
    initEditor();
    if(argc >= 2){
        editorOpen(argv[1]);
    }
    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}