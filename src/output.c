#include "common.h"
#include "data.h"
#include "prototypes.h"
/* We use a string buffer so we can write the data in one write statement
wirte statement is a system call that requires context switching between the current process
and the OS, but we can copy all the data into one place in the memory using procedure calls
(memory -> memory copy) and then write all the data in one write statement(system call)
*/
struct abuf{
    char *b;
    int len;
};
/*Intialize the buffer into Null and zero. The first realloc will do the same work as malloc*/
#define ABUF_INIT {NULL, 0};

void abAppend(struct abuf *ab, const char *s, int len){
    /*
    we create a new pointer that reserves a memory block from the end of the current
    text with length len(the new string length)
    */
    char *new = realloc(ab -> b, ab -> len + len);
    // We do a saftey check to make sure that if there is no enough space in the ram, end the program
    if(new == NULL) return;
    /* start the memory -> memory copy operation, the memcpy works as next 
    memcpy(destenation, source, number of bytes to copy(length))
    */
    memcpy(&new[ab -> len], s, len);
    /* We set our actual buffer pointer to our new pointer, and updates the length*/
    ab -> b = new;
    ab -> len += len;
}
/*When you use malloc (or realloc for a null pointer), the allocator reserves additional 8 or 16 bytes
called the header right before the pointer you get, that contains metadata (size, Magic number, and status {free, allocated})
so the free function goes the header and changes the status bit from allocated to free, so the system now can see this
space of memory available to write */
void abFree(struct abuf *ab){
    free(ab -> b);
}

/* This function is responsible about writing the lines to the terminal using the append buffer we created */
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


