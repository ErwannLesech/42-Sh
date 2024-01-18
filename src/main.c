/**
 * \file main.c
 * \brief Main file of the project (entry point)
 * \authors Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <err.h>

#include "ast/ast.h"
#include "execute/ast_eval.h"
#include "io_backend/io_backend.h"
#include "lexer/lexer.h"
#include "options/options.h"
#include "parser/parser.h"

/**
 * \fn int main(int argc, char **argv)
 * \brief Entry point of the program
 * \param argc Number of arguments
 * \param argv Arguments
 * \return the exit code of the program
 */
int main(int argc, char **argv)
{
    // Options handling
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    char *input = io_backend_manager(argc, argv);
    if (input == NULL)
    {
        // are you sure you want to exit with 127?
        errx(127, "Error while reading input");
    }

    struct lexer *lexer = lexer_new(input);
    if (lexer == NULL)
    {
        errx(1, "Error while creating lexer");
    }

    int val = parser_loop(lexer, pretty_print_enabled);
    if (val == 2)
    {
        fprintf(stderr, "Error while parsing\n");
    }


    lexer_free(lexer);
    free(input);
    return val;
}
