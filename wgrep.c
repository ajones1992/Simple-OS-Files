#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 0) {
        printf("wgrep: searchterm [file...]\n");
        return(1);
    } else if (argc == 1) {
        char stdBuff[7483647];
        while (fgets(stdBuff, 7483647, stdin) != NULL) {
            if (strstr(stdBuff, argv[1])) {
                printf("%s", stdBuff);
            }
        }
        return(0);
    }
    char buffer[7483647];
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            return(1);
        }
        while (fgets(buffer, 7483647, fp) != NULL) {
            if (strstr(buffer, argv[1])) {
                printf("%s", buffer);
            }
        }
    }
    return(0);
}