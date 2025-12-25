#include "common.h"
#include "data.h"
#include "prototypes.h"

/**
 * @brief Appends a new line of text to the editor's global row array.
 * @details This function performs a "Deep Copy" of a string into the editor's state. 
 * It first expands the primary array of 'erow' structures (The Directory) and then 
 * allocates a separate, dedicated memory block for the actual character data 
 * (The Content).
 * * @param s   Pointer to the source string (usually from a temporary buffer).
 * @param len The length of the string to be copied.
 */
void editorAppendRow(char *s, size_t len){
    /** * STEP 1: RESIZE THE ROW DIRECTORY
     * E.row is a pointer-to-pointer structure (an array of structs). 
     * We use a temporary pointer 'temp' to prevent a memory leak; if realloc 
     * fails, we don't want to lose our original E.row address.
     */
    
    void *temp = realloc(E.row, sizeof(erow) * (E.numRows + 1));
    if(temp == NULL) die(editorAppendRow);
    E.row = temp;

    /* 'at' marks the index of the newly created slot in the directory. */
    int at = E.numRows;
    E.row[at].size = len;

    /** * STEP 2: ALLOCATE THE CHARACTER CONTENT
     * We allocate a new, independent block of memory for the characters.
     * We add +1 to accommodate the null terminator ('\0'), ensuring the 
     * string is compatible with standard C string functions.
     */
    E.row[at].chars = malloc(len + 1);

    /* Copy the raw bytes from the source to our new permanent home in the heap. */
    memcpy(E.row[at].chars, s, len);

    /* Explicitly null-terminate the string for safety. */
    E.row[at].chars[len] = '\0';

    /* Increment the global count of rows now that the new row is fully initialized. */
    E.numRows++;
}
