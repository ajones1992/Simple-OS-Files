#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char buffer[100];
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("cannot open file\n");
            exit(1);
        }

        while (fgets(buffer, 100, fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    }
    return(0);
}
