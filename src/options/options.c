#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "options.h"

bool check_logger(int *argc, char **argv)
{
    bool logger_enabled = false;
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--logger") == 0)
        {
            logger_enabled = true;
            for (int j = i; j < *argc - 1; j++)
            {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
        }
    }
    return logger_enabled;
}

bool check_pretty_print(int *argc, char **argv)
{
    bool pretty_print_enabled = false;
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--pretty-print") == 0)
        {
            pretty_print_enabled = true;
            for (int j = i; j < *argc - 1; j++)
            {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
        }
    }
    return pretty_print_enabled;
}

void logger(char *str, enum logger_step step, bool logger_enabled)
{
    if(!str || !logger_enabled)
        return;

    switch (step)
    {
    case LOGGER_INPUT:
        printf("Input: %s\n", str);
        break;

    case LOGGER_PARSER:
        printf("Word_value: %s\n", str);
        break;

    case LOGGER_EXEC:
        printf("%s ", str);
        break;

    default:
        break;
    }
}   