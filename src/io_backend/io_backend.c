#include "io_backend.h"

/**
 * \brief Read from stdin
*/
char *io_backend_stdin(void)
{
    char *res = NULL;
    size_t totalSize = 0;
    ssize_t bytesRead;
    char *buff = NULL;
    size_t bufferSize = 256;

    buff = malloc(bufferSize);

    while ((bytesRead = read(STDIN_FILENO, buff + totalSize, bufferSize - totalSize)) > 0)
    {
        totalSize += bytesRead;
        if (totalSize >= bufferSize)
        {
            bufferSize *= 2;
            buff = realloc(buff, bufferSize);
            if (buff == NULL)
            {
                perror("Memory allocation error");
                return NULL;
            }
        }
    }

    if (bytesRead == -1)
    {
        perror("Error reading from stdin");
        free(buff);
        return NULL;
    }

    res = realloc(buff, totalSize + 1);
    res[totalSize] = '\0';

    return res;
}

/**
 * \brief Read from a file
*/
char *io_backend_file(char **argv)
{
    char *res = NULL;

    int fd = open(argv[2], O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return NULL;
    }

    size_t totalSize = 0;
    ssize_t bytesRead;
    char *buff = NULL;
    size_t bufferSize = 256;

    buff = malloc(bufferSize);

    while ((bytesRead = read(fd, buff + totalSize, bufferSize - totalSize)) > 0)
    {
        totalSize += bytesRead;
        if (totalSize >= bufferSize)
        {
            bufferSize *= 2;
            buff = realloc(buff, bufferSize);
            if (buff == NULL)
            {
                perror("Memory allocation error");
                close(fd);
                return NULL;
            }
        }
    }

    if (bytesRead == -1)
    {
        perror("Error reading from file");
        free(buff);
        close(fd);
        return NULL;
    }

    res = realloc(buff, totalSize + 1);
    res[totalSize] = '\0';
    close(fd);

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

    char *input = malloc(strlen(argv[1]) + 1);

    strcpy(input, argv[1]);

    return input;
}
