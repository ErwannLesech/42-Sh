/**
 * \file options.h
 * \brief Header for options functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <stdio.h>

#include "ast/ast.h"

/**
 * \brief Check if the pretty print option is asked in args.
 * \param argc The number of args.
 * \param argv The args.
 * \return True if the pretty print option is inside of args, false otherwise.
 */
bool check_pretty_print(int *argc, char **argv);

/**
 * \brief Pretty printf of ast.
 * \param ast The ast to print.
 * \param pretty_print_enabled True if the pretty print is enabled, false otherwise.
 * \param depths The depths of the ast.
 */
void pretty_print(struct ast_node *ast, bool pretty_print_enabled, int *depths);

/**
 * \brief Check if the given string is a number.
 * \param str The string to check.
 * \return True if the string is a number, false otherwise.
 */
int is_number(char *str);

#endif /* ! OPTIONS_H  */
