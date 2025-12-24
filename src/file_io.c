#include "common.h"
#include "data.h"
#include "prototypes.h"


/**
 * @brief Bridges the gap between the persistent storage (disk) and the editor memory.
 * @details Opens a file, reads its contents line-by-line using the modern getline() 
 * function, and populates the editor's row structures. It handles newline stripping 
 * to ensure the internal text representation is clean.
 * * @param fileName The path to the file to be loaded.
 */void editorOpen(char *fileName){
    /* Open the file in read-only mode. fopen returns a pointer to a FILE struct 
     * which acts as a "management office" for the file stream. 
     * We used a pointer so we can avoid the overhead of copying the struct each time when we 
     * pass it to functions
     * The file struct is a case of the Opaque struct, you can read more here:
     * https://en.wikipedia.org/wiki/Opaque_data_type
     * https://en.wikipedia.org/wiki/Opaque_pointer
     */
    FILE *fp = fopen(fileName, "r");
    /* Error Guard: If the file doesn't exist or is inaccessible, exit gracefully. */
    if (!fp) die("fopen");
    /* Initialize the line buffer variables for getline(). Setting line to NULL 
       tells getline() to perform the initial malloc() for us. */

    char *line = NULL;

    size_t lineCap = 0;
    ssize_t lineLen;
    /** * Main File Reading Loop: 
     * getline() is used for its "Auto-Expanding" logic. It will automatically 
     * call realloc() if it encounters a line longer than lineCap.
     * It returns -1 when it hits the End Of File (EOF).
     */
    while((lineLen = getline(&line, &lineCap, fp)) != -1){
        if(lineLen != -1){
            /* LINE STRIPPING:
           getline() includes the \n or \r\n characters in its count. We loop 
           backward from the end of the string to "chop off" these control 
           characters so our editor stores only the raw text. */
            while(lineLen > 0 && (line[lineLen - 1] == '\n' ||
                                    line[lineLen - 1] == '\r'))
                lineLen--;
        }
        /* Create a new row in our global state and copy the clean text into it. */
        editorAppendRow(line, lineLen);
    }

    /** * HOUSEKEEPING:
     * 1. Release the line buffer created by getline() to avoid a memory leak.
     * 2. Close the file pointer to release the system's file handle.
     */
    free(line);
    fclose(fp);

    /**
     * The fclose performs three critical steps:
     * 1. Flushing the buffer
     * 2. Releasing file descriptors
     * 3. Deallocating the FILE Struct
     * You can read some answers here, it's pretty good tho:
     * "https://stackoverflow.com/questions/5130375/how-exactly-does-fopen-fclose-work"
     */

}
