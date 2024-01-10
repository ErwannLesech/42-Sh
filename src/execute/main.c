#include "ast_eval.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

int main ()
{
    // "if false; then elif true; then echo hello; else echo world; fi"
    struct lexer *lexer = lexer_new("if false; then elif true; then echo hello; else echo world; fi");
    struct ast_node *node = parse(lexer);
    //print_ast(node, 0);
    match_ast(node);
    ast_free(node);
    lexer_free(lexer);
    return 0;
}