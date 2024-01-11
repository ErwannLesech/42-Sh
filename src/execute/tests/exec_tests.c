#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../../parser/parser.h"
#include "../ast_eval.h"
#include "../../lexer/lexer.h"
#include "../../io_backend/io_backend.h"

TestSuite(exec, .timeout = 1);

Test(exec, test_exec_simple_command)
{
    char *argv[] = {"./42sh", "-c", "echo test", NULL};
    int argc = 3;

    char *input = io_backend(argc, argv);

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