#include "common.h"
#include "data.h"
#include "prototypes.h"

/**
 * @brief Updates the logical cursor position based on directional input.
 * @details This function modifies the global editor state (E.cx, E.cy). It includes 
 * "Boundary Guarding" to ensure the cursor cannot move outside the visible 
 * bounds of the terminal screen.
 * @param key The integer representation of the arrow key (from the editorKey enum).
 */
void editorMoveCursor(int key){
    switch (key)
    {
    case ARROW_LEFT:
        if(E.cx > 0)
            E.cx--;
        break;
    case ARROW_RIGHT:
        if(E.cx < E.screenCols - 1)
            E.cx++;
        break;
    case ARROW_UP:
        if(E.cy > 0)
            E.cy--;
        break;
    case ARROW_DOWN:
        if(E.cy < E.screenRows - 1)
            E.cy++;
        break;
    }
}
/**
 * @brief The main Input Dispatcher that maps keypresses to editor actions.
 * @details This function calls editorReadKey() to get an input and then 
 * routes that input to the correct logic (cursor movement, scrolling, or exit).
 */void editorProcessKeypress(){
    int c = editorReadKey();
    switch(c) {
        case CTRL_KEY('q'):
        /** * CLEAN EXIT:
             * Before closing the program, we wipe the screen (\x1b[2J) and 
             * reset the cursor to the home position (\x1b[H). This leaves 
             * the user's terminal in a clean state, rather than leaving 
             * the editor's text visible. 
             */
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
        case HOME_KEY:
            /* Teleport cursor to the start of the current line. */
            E.cx = 0;
            break;
        case END_KEY:
        /* Teleport cursor to the edge of the screen width. */
            E.cx = E.screenCols - 1;
            break;
            /** Page up and Page down will move you between the first and last rows */
        case PAGE_UP:
        case PAGE_DOWN:
        /** * SCROLL SIMULATION:
             * Page movements are simulated by calling editorMoveCursor 
             * for every single row currently visible on the screen. 
             */
            {
                int times = E.screenRows;
                while(times--){
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }   
            }
            break;

        /* Directional Input Routing */
        case ARROW_UP:
        case ARROW_LEFT:
        case ARROW_DOWN:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
    }

}
