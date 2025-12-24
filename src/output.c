#include "common.h"
#include "data.h"
#include "prototypes.h"
/**
 * @struct abuf
 * @brief A dynamic string buffer for batched terminal output.
 * * DESIGN RATIONALE:
 * - SYSTEM CALL REDUCTION: The write() function is a System Call that triggers a 
 * costly context switch between the User Process and the Kernel. 
 * - BATCHED I/O: Instead of multiple small writes, we use fast User-Space 
 * procedure calls (memcpy) to accumulate data in RAM.
 * - ATOMIC REFRESH: By performing a single write() of the entire buffer, we 
 * eliminate screen flickering and improve rendering performance.
 */
struct abuf{
    char *b;        /**< Pointer to the buffer in memory */
    int len;        /**< Current length of the data in the buffer */
};
/**
 * @def ABUF_INIT
 * @brief Initializer for an empty buffer.
 * Setting .b to NULL ensures the first call to realloc() behaves like malloc().
 */#define ABUF_INIT {NULL, 0};


 /**
 * @brief Appends a string to the dynamic buffer.
 * * @param ab Pointer to the abuf structure.
 * @param s The string to be appended.
 * @param len The number of bytes to copy from 's'.
 */
void abAppend(struct abuf *ab, const char *s, int len){
    /* 1. ATTEMPT RESIZE: realloc() tries to expand the existing block or moves 
          it to a new location in RAM large enough to hold the combined data. */
    char *new = realloc(ab -> b, ab -> len + len);
    /* 2. ALLOCATION SAFETY: If realloc returns NULL, the system is out of memory. 
          We return early to prevent a crash (Segmentation Fault). */    
    if(new == NULL) return;
    /* 3. BATCHED COPY: We copy 's' into the 'newBuffer' block starting exactly 
          where the previous data ended (&newBuffer[ab -> len]). */
    memcpy(&new[ab -> len], s, len);
    /* 4. STATE UPDATE: Point the buffer to the new memory address and update length. */    
    ab -> b = new;
    ab -> len += len;
}
/**
 * @brief Releases the memory used by the dynamic buffer.
 * * @details The Heap Manager identifies the block size by reading the hidden 
 * metadata header (containing size and status) located just before the pointer 
 * address. Calling free() marks this block as 'available' in the free list.
 */
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


