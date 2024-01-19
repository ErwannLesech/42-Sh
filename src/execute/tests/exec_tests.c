/**
 * \file exec_tests.c
 * \brief Tests the exec function.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "execute/ast_eval.h"
#include "io_backend/io_backend.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

TestSuite(exec, .timeout = 10);

Test(exec, test_exec_simple_command)
{
    char *argv[] = { "./42sh", "-c", "echo test", NULL };
    int argc = 3;

    char *input = io_backend_manager(argc, argv);

    struct lexer *lexer = lexer_new(input);

    struct ast_node *ast = parse(lexer);

    cr_redirect_stdout();

    match_ast(ast);
    fflush(stdout);
    cr_assert_stdout_eq_str("test\n");

    lexer_free(lexer);
    ast_free(ast);
    free(input);
}