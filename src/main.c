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
    bool logger_enabled = check_logger(&argc, argv);
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    char *input = io_backend_manager(argc, argv);
    if (input == NULL)
    {
        errx(127, "Error while reading input");
    }

    logger(input, LOGGER_INPUT, logger_enabled);

    struct lexer *lexer = lexer_new(input);

    int val = parser_loop(lexer, logger_enabled, pretty_print_enabled);
	if (val == 2)
	{
		fprintf(stderr, "Error while parsing\n");
	}
    lexer_free(lexer);
    free(input);
    return val;
}
