#include "parser.h"

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
    free(tok.data);
    return tok.type;
}

enum token_type parser_pop(struct lexer *lexer)
{
    struct token tok = lexer_pop(lexer);
    free(tok.data);
    return tok.type;
}

struct ast_node *parse(struct lexer *lexer)
{
    return input(lexer);
}
