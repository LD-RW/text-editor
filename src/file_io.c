#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void editorOpen(char *fileName){
    free(E.fileName);
    E.fileName = strdup(fileName);
    FILE *fp = fopen(fileName, "r");
    if (!fp) die("fopen");

    char *line = NULL;
    size_t lineCap = 0;
    ssize_t lineLen;
    while((lineLen = getline(&line, &lineCap, fp)) != -1){
        if(lineLen != -1){
            while(lineLen > 0 && (line[lineLen - 1] == '\n' || line[lineLen - 1] == '\r'))
                lineLen--;
        }
        editorInsertRow(E.numRows, line, lineLen);
    }
    free(line);
    fclose(fp);
    E.dirty = 0;
}

char *editorRowsToString(int *bufLen){
    int totalLen = 0;
    int j;
    for(j = 0; j < E.numRows; ++j){
        totalLen += E.row[j].size + 1;
    }
    *bufLen = totalLen;

    char *buf = malloc(totalLen);
    char *p = buf;
    for(j = 0; j < E.numRows; ++j){
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }
    return buf;
}

void editorSave(){
    if(E.fileName == NULL) {
        E.fileName = editorPrompt("Save as: %s (ESC to cancel)");
        if(E.fileName == NULL){
            editorSetStatusMessage("Save aborted");
            return;
        }
    }

    int len;
    char *buf = editorRowsToString(&len);
    
    int fd = open(E.fileName, O_RDWR | O_CREAT, 0644);
    if(fd != -1){
        if(ftruncate(fd, len) != -1){
            if(write(fd, buf, len) == len){
                close(fd);
                free(buf);
                E.dirty = 0;
                editorSetStatusMessage("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));

}