#include "../include/common.h"
#include "../include/data.h"
#include "../include/prototypes.h"

void editorOpen(char *fileName){
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
        editorAppendRow(line, lineLen);
    }
    free(line);
    fclose(fp);
}