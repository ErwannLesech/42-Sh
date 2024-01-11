#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <stdbool.h>

enum logger_step
{
    LOGGER_INPUT,
    LOGGER_PARSER,
    LOGGER_EXEC,
};

/**
 * \brief Check if the logger option is asked in args.
*/
bool check_logger(int *argc, char **argv);

/**
 * \brief Check if the pretty print option is asked in args.
*/
bool check_pretty_print(int *argc, char **argv);

/**
 * \brief Pretty printf of string.
*/
void logger(char *str, enum logger_step step, bool logger_enabled);

#endif /* ! OPTIONS_H  */
