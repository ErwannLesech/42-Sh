#include "parser/parser.h"
#include "lexer/lexer.h"
#include "ast/ast.h"
#include <stdio.h>



int main(void)
{
    struct lexer *lexer = lexer_new("if false; true; then\necho a\necho b; elif false; then\n echo e; else echo c;\nfi");
    //struct lexer *lexer = lexer_new("echo foo; echo bar");
    struct ast_node *ast = parse(lexer);
    printf("AST:\n");
    print_ast(ast, 0);
}