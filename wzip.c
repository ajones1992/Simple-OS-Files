#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc == 0) {
        printf("wzip: file1 [file2...]\n");

        return(1);
    }

    char *buffer;
    int charsRead;
    size_t line_buf_size = 0;
    int counter = 1;
    char currentCH;

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return(1);
        }
        while ((charsRead = getline(&buffer, &line_buf_size, fp)) != -1) {
            currentCH = buffer[0];
            for (int ch = 1; ch < sizeof(buffer); ch++) {
                if (currentCH == buffer[ch]) {
                    counter++;
                } else {
                    fwrite(&counter, 4, 1, stdout);
                    if (counter > 1) {
                        counter = 1;
                    }
                    fwrite(&currentCH, 1, 1, stdout);
                    currentCH = buffer[ch];
                }
            }
        }
    }
    return(0);
}
