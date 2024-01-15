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
    if (parser_peek(lexer) == TOKEN_EOL || parser_peek(lexer) == TOKEN_EOF
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
            // CHECK IF END OR ERROR
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
    struct ast_node *current = ast_node_new(AST_AND_OR);
    struct ast_node *child = pipeline(lexer);
    if (child != NULL)
    {
        ast_append(current, child);
        while (parser_peek(lexer) == TOKEN_AND || parser_peek(lexer) == TOKEN_OR)
        {
            if (parser_peek(lexer) == TOKEN_AND)
                ast_append(current, ast_node_word("&&"));
            else
                ast_append(current, ast_node_word("||"));
            parser_pop(lexer);
            while (parser_peek(lexer) == TOKEN_EOL)
                parser_pop(lexer);
            child = pipeline(lexer);
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

struct ast_node *pipeline(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_PIPELINE);
    if (parser_peek(lexer) == TOKEN_NEGATE)
    {
        ast_append(current, ast_node_new(AST_NEGATE));
    }
    struct ast_node *child = command(lexer);
    if (child != NULL)
    {
        ast_append(current, child);
        while (parser_peek(lexer) == TOKEN_PIPE)
        {
            parser_pop(lexer);
            while (parser_peek(lexer) == TOKEN_EOL)
                parser_pop(lexer);
            child = command(lexer);
            if (child == NULL)
            {
                ast_free(current);
                return NULL;
            }
            ast_append(current, child);
        }
        return current;
    }
    return command(lexer);
}

struct ast_node *command(struct lexer *lexer)
{
    struct ast_node *current = simple_command(lexer);
    if (current != NULL)
        return current;
    current = shell_command(lexer);
    if (current != NULL)
    {
        struct ast_node *child = redirection(lexer);
        while (child != NULL)
        {
            ast_append(current, child);
            child = redirection(lexer);
        }
        return current;
    }
    return NULL;
}

struct ast_node *simple_command(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_SIMPLE_COMMAND);
    struct ast_node *child = prefix(lexer);
    if (child != NULL)
    {
        ast_append(current, child);
        struct ast_node *child2 = prefix(lexer);
        while (child2 != NULL)
        {
            ast_append(current, child2);
            child2 = prefix(lexer);
        }
    }
    if (parser_peek(lexer) == TOKEN_WORD)
    {
        char *value = lexer_peek(lexer).data;
        if (strcmp(value, "while") == 0 || strcmp(value, "until") == 0 ||
            strcmp(value, "for") == 0 || strcmp(value, "do") == 0)
        {
            free(value);
            ast_free(current);
            return NULL;
        }
        free(value);
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
    struct ast_node *current = redirection(lexer);
    if (current != NULL)
        return current;
    if (parser_peek(lexer) == TOKEN_WORD || parser_peek(lexer) == TOKEN_IF
        || parser_peek(lexer) == TOKEN_THEN || parser_peek(lexer) == TOKEN_ELSE
        || parser_peek(lexer) == TOKEN_ELIF || parser_peek(lexer) == TOKEN_FI
        || parser_peek(lexer) == TOKEN_DONE)
    {
        struct ast_node *curr = ast_node_word(lexer_peek(lexer).data);
        parser_pop(lexer);
        return curr;
    }
    return NULL;
}