/**
 * \file options.h
 * \brief Header for options functions.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <stdio.h>

#include "ast/ast.h"

/**
 * \brief Enum for the different logger steps.
 */
enum logger_step
{
    LOGGER_INPUT,
    LOGGER_PARSER,
    LOGGER_EXEC,
};

/**
 * \brief Check if the logger option is asked in args.
 * \param argc The number of args.
 * \param argv The args.
 * \return True if the logger option is inside of args, false otherwise.
 */
bool check_logger(int *argc, char **argv);

/**
 * \brief Check if the pretty print option is asked in args.
 * \param argc The number of args.
 * \param argv The args.
 * \return True if the pretty print option is inside of args, false otherwise.
 */
bool check_pretty_print(int *argc, char **argv);

/**
 * \brief Pretty printf of string.
 * \param str The string to print.
 * \param step The step of the logger.
 * \param logger_enabled True if the logger is enabled, false otherwise.
 */
void logger(char *str, enum logger_step step, bool logger_enabled);

/**
 * \brief Pretty printf of ast.
 */
void pretty_print(struct ast_node *ast, bool pretty_print_enabled, int *depths);

/**
 * \brief Check if the given string is a number.
 * \param str The string to check.
 * \return True if the string is a number, false otherwise.
 */
int is_number(char *str);

#endif /* ! OPTIONS_H  */
