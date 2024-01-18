/**
 * \file parser.c
 * \brief Parse the input and build the AST.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "parser.h"

#include "execute/ast_eval.h"

/*
input =
      list '\n'
    | list EOF
    | '\n'
    | EOF
    ;

list = and_or { ';' and_or } [ ';' ] ;
and_or = pipeline ;
pipeline = command ;

command =
      simple_command
    | shell_command
    ;

shell_command = rule_if ;
rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
else_clause =
      'else' compound_list
    | 'elif' compound_list 'then' compound_list [else_clause]
    ;

compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;

simple_command = WORD { element } ;
element = WORD ;

*/

enum token_type parser_peek(struct lexer *lexer)
{
    struct token tok = lexer_peek(lexer);
    if (tok.type != TOKEN_ERROR)
        free(tok.data);
    return tok.type;
}

enum token_type parser_pop(struct lexer *lexer)
{
    struct token tok = lexer_pop(lexer);
    if (tok.type != TOKEN_ERROR)
        free(tok.data);
    return tok.type;
}

struct ast_node *parse(struct lexer *lexer)
{
    return input(lexer);
}

/**
 * \brief Parse loop line by line and execute it
 */
int parser_loop(struct lexer *lexer, bool pretty_print_enabled)
{
    int return_value = 0;
    while (parser_peek(lexer) != TOKEN_EOF)
    {
        struct ast_node *ast = parse(lexer);
        if (ast == NULL)
        {
            return 2;
        }

        if (pretty_print_enabled)
        {
            print_ast(ast, 0);
            int depths = 0;
            pretty_print(ast, pretty_print_enabled, &depths);
        }

        if (ast->type != AST_EMPTY)
        {
            return_value = match_ast(ast);
        }
        ast_free(ast);
        
        if (return_value != 0 && return_value != 1)
        {
            fprintf(stderr, "Error while executing\n");
        }
    }

    return return_value;
}
