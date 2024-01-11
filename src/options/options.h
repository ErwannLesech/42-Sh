#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <stdbool.h>

/**
 * \brief Check if the logger option is asked in args
*/
bool check_logger(int *argc, char **argv);

/**
 * \brief Check if the pretty print option is asked in args
*/
bool check_pretty_print(int *argc, char **argv);

#endif /* ! OPTIONS_H  */
