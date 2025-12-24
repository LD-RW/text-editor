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
/**
 * @brief Renders each row of the text buffer or the welcome screen to the internal buffer.
 * @details This function iterates through the terminal's vertical resolution (screenRows).
 * It handles three distinct states: drawing file content, drawing the centered 
 * welcome message, or drawing the 'empty' file indicators (~).
 * * @param ab Pointer to the dynamic append buffer (abuf) used to batch terminal output.
 */
void editorDrawRows(struct abuf *ab){
    int y; /* The current vertical screen row being processed (0-indexed) */
    for(y = 0; y < E.screenRows; ++y){
        /* CASE 1: DRAWING BEYOND THE FILE END (Empty rows or Welcome message) */
        if(y >= E.numRows) {
            /* Logic for centering the Welcome message on a blank editor */
            if(y == E.screenRows / 3 && E.numRows == 0){
            char welcome[80];
            /* snprintf: Formats the version string safely into the 'welcome' buffer.
             * Unlike sprintf, it respects the destination size to prevent buffer overflows. */
            int welcomeLen = snprintf(welcome, sizeof(welcome),
                                        "B-textEditor --Version %s", B_TEXTEDITOR_VERSION);
            /* CLIPPING: Ensure text doesn't bleed past the screen width */
            if(welcomeLen > E.screenCols){
                welcomeLen = E.screenCols;
            }
            /* CENTERING MATH: Calculate horizontal leading space */
            int padding  = (E.screenCols - welcomeLen) / 2;
            /** Draw the tilde on the line, and subtract the distance from the middle by 1 */
            if(padding){
                abAppend(ab, "~", 1);   /* Leading Vim-style tilde */
                padding--;
            }
            /* Append the calculated spacing to push text to the center */
            while(padding--) abAppend(ab, " ", 1);
            abAppend(ab, welcome, welcomeLen);
            }
            else{
                /* Regular empty row indicator */
                abAppend(ab, "~", 1);
            }
        }
        /* CASE 2: DRAWING ACTUAL FILE CONTENT */
        else{ 
            int len = E.row[y].size;                      

            /* CLIPPING: Only draw characters that fit within the horizontal bounds */
            if(len > E.screenCols) len = E.screenCols; 
            abAppend(ab, E.row[y].chars, len);
        }
        /**
         * ANSI EL (Erase In Line) - \x1b[K:
         * Clears from the current cursor position to the end of the line.
         * This prevents "ghosting" (leftover characters from the previous frame 
         * showing up behind shorter lines). We batch these 3 bytes into abuf 
         * for efficiency rather than clearing the whole screen, which causes flicker.
         */

        abAppend(ab, "\x1b[K", 3);

        /* Only move the cursor down if we are NOT on the very last screen row.
         * Sending \r\n on the final row triggers a terminal scroll-up, 
         * which would ruin the UI alignment. */
        if(y < E.screenRows - 1){
            abAppend(ab, "\r\n", 2);
        }
    }
}


/**
 * @brief Orchestrates a clean, flicker-free refresh of the terminal display.
 * * @details This function implements a "Double Buffering" pattern. It collects all 
 * visual updates—including screen clearing, row rendering, and cursor positioning—into 
 * a single memory buffer before executing one atomic write to the terminal. This 
 * eliminates the "stuttering" effect caused by multiple slow system calls.
 */
void editorRefreshScreen(){
    /* Initialize a dynamic append buffer to batch all terminal instructions. */
    struct abuf ab = ABUF_INIT;

    /* 1. HIDE CURSOR: Prevents the user from seeing the cursor "fly" across the 
          screen as individual rows are drawn (eliminates cursor flickering). */
    abAppend(&ab, "\x1b[?25l", 6);

    /* 2. RESET POSITION: Teleports the "pen" to the Home position (1,1) so the 
          next frame perfectly overwrites the previous one. */
    abAppend(&ab, "\x1b[H", 3);

    /* 3. RENDER CONTENT: Append all text rows and UI elements to the buffer. */
    editorDrawRows(&ab);

    /* 4. CURSOR TRANSLATION: Prepare the command to place the cursor at the user's
          logical position. We use a temporary buffer for string formatting. */
    char buf[32];
    /* Convert 0-indexed C coordinates to 1-indexed terminal coordinates. */
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);

    /* Append the specific cursor placement to the batch. We use strlen() because 
       the length of the command changes based on the number of digits in cx/cy. */
    abAppend(&ab, buf, strlen(buf));

    /* 5. SHOW CURSOR: Reveal the blinking cursor now that the frame is complete. */
    abAppend(&ab, "\x1b[?25h", 6);

    /* 6. ATOMIC UPDATE: Flush the entire frame to the hardware in one system call. */
    write(STDOUT_FILENO, ab.b, ab.len);

    /* 7. CLEANUP: Release the heap memory to prevent memory leaks. */
    abFree(&ab);
}


