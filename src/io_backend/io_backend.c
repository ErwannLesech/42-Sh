#include "io_backend.h"

char *io_backend(int argc, char **argv)
{
    char *res = NULL;

    if (argc < 2)
    {
        char *result = NULL;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        size_t totalSize = 0;
        char *buff = NULL;

        while ((read = getline(&line, &len, stdin)) != -1)
        {
            buff = realloc(buff, totalSize + read + 1);
            memcpy(buff + totalSize, line, read);
            totalSize += read;
        }

        buff = realloc(buff, totalSize + 1);
        buff[totalSize] = '\0';

        free(line);
        res = buff;
    }
    else if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        size_t totalSize = 0;
        char *buff = NULL;

        fp = fopen(argv[2], "r");
        if (fp == NULL)
        {
            printf("Error: file not found\n");
            return NULL;
        }

        while ((read = getline(&line, &len, fp)) != -1)
        {
            buff = realloc(buff, totalSize + read + 1);
            memcpy(buff + totalSize, line, read);
            totalSize += read;
        }

        buff = realloc(buff, totalSize + 1);
        buff[totalSize] = '\0';

        free(line);
        fclose(fp);
        res = buff;
    }
    else
    {
        res = strdup(argv[1]);
    }

    return res;
}
