#include "io_backend.h"


/**
 * \brief Read from stdin
*/
char *io_backend_stdin(void)
{
    char *res = NULL;
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

    return res;
}

/**
 * \brief Read from a file
*/
char *io_backend_file(char **argv)
{
    char *res = NULL;

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

    return res;
}

char *io_backend(int argc, char **argv)
{
    if (argc < 2)
    {
        return io_backend_stdin();
    }
    else if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        return io_backend_file(argv);
    }

    char *input = strdup(argv[1]);

    return input;
}
