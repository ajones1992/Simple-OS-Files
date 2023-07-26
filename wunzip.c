#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("wunzip: file1 [file2...]\n");
        return (1);
    }

    int itemsRead;
    int quantity;
    char charRead;

    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL)
        {
            printf("wunzip: cannot open file\n");
            return (1);
        }

        while ((itemsRead = fread(&quantity, 4, 1, fp)) == 1)
        {
            fread(&charRead, 1, 1, fp);
            for (int j = 0; j < quantity; j++) {
                fprintf(stdout, "%c", charRead);
            }
        }
    }
    return(0);
}