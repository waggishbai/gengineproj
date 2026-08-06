#ifndef _STDIO_H
#include <stdio.h>
#endif

#include <stdlib.h>

//this is just a gimick i made because i thought it was funny :3
#ifdef HAI_IMP_
#define HAI_IMP_

void hai() {
    printf("hai\n");
}

#endif

/*this is the actual code. it's just a simple text file loader, it reads the input file and outputs a const char, define LOAD_IMP_ before including to use it.*/

#ifndef LOAD_H
#define LOAD_H

const char* load(const char* filename) {
    FILE* f;
    f = fopen(filename, "r");
    char c;
    char *buff = (char*)malloc(0);
    int bsize = 0;
    while ((c = fgetc(f)) != EOF) {
        bsize++;
        buff = (char*)realloc(buff, sizeof(char) * bsize);
        buff[bsize - 1] = c;
    }
    bsize++;
    buff = (char*)realloc(buff, sizeof(char) * bsize);
    buff[bsize - 1] = '\0';    
    const char* out = buff;
    fclose(f);
    free(buff);
    buff = nullptr;
    return out;
}

#endif

//              _____
//              \    /
//               \  /
//                \/
//                |
//                |
//                |
