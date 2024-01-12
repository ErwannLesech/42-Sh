/**
 * \file parser_element.c
 * \brief Parse the element rule.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
*/

#include "parser.h"

struct ast_node *input(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_EOL)
    {
        parser_pop(lexer);
        return ast_node_new(AST_EMPTY);
    }
    if (parser_peek(lexer) == TOKEN_EOF)
        return ast_node_new(AST_EMPTY);
    struct ast_node *node = list(lexer);
    if (parser_peek(lexer) == TOKEN_EOL
        || parser_peek(lexer) == TOKEN_EOF
        || parser_peek(lexer) == TOKEN_SEMICOLON)
    {
        return node;
    }
    ast_free(node);
    return NULL;
}

struct ast_node *list(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_COMMAND_LIST);
    struct ast_node *child = and_or(lexer);
    if (child != NULL)
    {
        ast_append(current, child);
        while (parser_peek(lexer) == TOKEN_SEMICOLON)
        {
            parser_pop(lexer);
            if (parser_peek(lexer) == TOKEN_EOL 
            || parser_peek(lexer) == TOKEN_EOF)
                return current;
            child = and_or(lexer);
            //CHECK IF END OR ERROR
            if (child == NULL)
            {
                ast_free(current);
                return NULL;
            }
            ast_append(current, child);
        }
        return current;
    }
    ast_free(current);
    return NULL;
}

struct ast_node *and_or(struct lexer *lexer)
{
    return pipeline(lexer);
}

struct ast_node *pipeline(struct lexer *lexer)
{
    return command(lexer);
}

struct ast_node *command(struct lexer *lexer)
{
    struct ast_node *current = simple_command(lexer);
    if (current != NULL)
        return current;
    current = shell_command(lexer);
    if (current != NULL)
        return current;
    return NULL;
}

struct ast_node *simple_command(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_SIMPLE_COMMAND);
    if (parser_peek(lexer) == TOKEN_WORD)
    {
        struct ast_node *new = ast_node_word(lexer_peek(lexer).data);
        parser_pop(lexer);
        ast_append(current, new);
        struct ast_node *curr = element(lexer);
        while (curr != NULL)
        {
            ast_append(current, curr);
            curr = element(lexer);
        }
        return current;
    }
    ast_free(current);
    return NULL;
}

struct ast_node *element(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_WORD || parser_peek(lexer) == TOKEN_IF || parser_peek(lexer) == TOKEN_THEN
        || parser_peek(lexer) == TOKEN_ELSE || parser_peek(lexer) == TOKEN_ELIF || parser_peek(lexer) == TOKEN_FI)
    {
        struct ast_node *curr = ast_node_word(lexer_peek(lexer).data);
        parser_pop(lexer);
        return curr;
    }
    return NULL;
}
