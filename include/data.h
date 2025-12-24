#ifndef DATA_H
#define DATA_H

#include<termios.h>

/**
 * @struct erow
 * @brief A container representing a single line of text in the editor.
 * * Stores the memory address and the explicit byte length of a line.
 * * @details
 * - **Binary Safety:** By storing 'size' explicitly, we can handle lines containing 
 * embedded null bytes (e.g., "Hello \0 World") that would otherwise truncate data.
 * - **Performance:** Allows the use of memcpy() instead of strcpy(). This avoids 
 * the O(n) overhead of scanning for a null terminator during every operation.
 * - **Security:** Prevents infinite loops and program crashes caused by missing 
 * terminators, effectively mitigating potential buffer overflow vulnerabilities.
 */
typedef struct erow{
    int size;           /* Number of bytes in the line */
    char *chars;        /* Pointer to the character data on the heap */
} erow;
/**
 * @struct editorConfig
 * @brief The "Global State" or central brain of the application.
 * * This structure acts as a single source of truth, synchronized across all 
 * source files to maintain the editor's current environment and data.
 */
struct editorConfig{
    /** [cx, cy]: Cursor coordinates. Tracks the user's position within the text buffer. */
    int cx, cy;
    /** [screenRows, screenCols]: Display dimensions. Used for UI clipping and 
        ensuring the editor does not render beyond the terminal's visible bounds. */
    int screenRows;
    int screenCols;
    /** numRows: Total line count of the currently loaded file. */
    int numRows;
    /** *row: Pointer to the dynamic array of 'erow' structures (the file content). */
    erow *row;
    /** orig_termios: A snapshot of the terminal's state prior to entering 'Raw Mode.' 
        Used to restore the user's original environment upon program exit. */
    struct termios orig_termios;

};
/**
 * @brief External declaration of the global editor state.
 * * DESIGN RATIONALE:
 * - PREVENTING REDEFINITION: Defining 'struct editorConfig E' in a header included 
 * by multiple .c files would cause a "multiple definition" error at the linking stage.
 * - GLOBAL VISIBILITY: The 'extern' keyword acts as a promise to the compiler that the 
 * storage for 'E' is allocated in exactly one translation unit (data.c).
 * - DATA SYNCHRONIZATION: This allows all program modules (editor.c, terminal.c, etc.) 
 * to access and modify the same instance of the global state in memory.
 */
extern struct editorConfig E;




#endif