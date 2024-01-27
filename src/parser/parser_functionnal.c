/**
 * \file parser_functionnal.c
 * \brief Parse the functionnal
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "parser.h"

struct ast_node *shell_command(struct lexer *lexer)
{
    struct ast_node *current = rule_if(lexer);
    if (current == NULL)
        current = rule_while(lexer);
    if (current == NULL)
        current = rule_until(lexer);
    if (current == NULL)
        current = rule_for(lexer);
    return current;
}

struct ast_node *prefix(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_WORD_ASSIGNMENT)
    {
        struct ast_node *current = ast_node_new(AST_WORD_ASSIGNMENT);
        current->value = lexer_peek(lexer).data;
        parser_pop(lexer);
        return current;
    }
    return redirection(lexer);
}

struct ast_node *redirection(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_REDIRECTION);
    if (parser_peek(lexer) == TOKEN_IONUMBER)
    {
        struct ast_node *io_number = ast_node_new(AST_IONUMBER);
        io_number->value = lexer_pop(lexer).data;
        ast_append(current, io_number);
    }
    if (parser_peek(lexer) == TOKEN_REDIR)
    {
        current->value = lexer_peek(lexer).data;
        parser_pop(lexer);
        if (parser_peek(lexer) == TOKEN_WORD)
        {
            ast_append(current, ast_node_word(lexer_peek(lexer).data));
            parser_pop(lexer);
            return current;
        }
    }
    ast_free(current);
    return NULL;
}
