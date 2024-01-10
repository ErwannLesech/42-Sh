#include "parser.h"

struct ast_node *shell_command(struct lexer *lexer)
{
    return rule_if(lexer);
}

struct ast_node *rule_if(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_CONDITION);
    if (parser_peek(lexer) == TOKEN_IF)
    {
        parser_pop(lexer);
        struct ast_node *condition = compound_list(lexer);
        if (condition == NULL)
            goto ERROR;
        ast_append(current, condition);
        if (parser_peek(lexer) != TOKEN_THEN)
            goto ERROR;
        parser_pop(lexer);
        struct ast_node *response = compound_list(lexer);
        if (response == NULL)
            goto ERROR;
        ast_append(current, response);
        struct ast_node *else_c = else_clause(lexer);
        if (else_c == NULL)
            goto ERROR;
        else if (else_c->type != AST_EMPTY)
            ast_append(current, else_c);
        else
            ast_free(else_c);
        if (parser_peek(lexer) != TOKEN_FI)
            goto ERROR;
        parser_pop(lexer);
        return current;
    }
ERROR:
    ast_free(current);
    return NULL;
}

struct ast_node *else_clause(struct lexer *lexer)
{
    if (parser_peek(lexer) == TOKEN_ELSE)
    {
        parser_pop(lexer);
        struct ast_node *current = compound_list(lexer);
        if (current == NULL)
            goto ERROR;
        return current;
    }
    else if (parser_peek(lexer) == TOKEN_ELIF)
    {
        parser_pop(lexer);
        struct ast_node *current = ast_node_new(AST_CONDITION);
        struct ast_node *condition = compound_list(lexer);
        if (condition == NULL)
            goto ERROR;
        ast_append(current, condition);
        if (parser_peek(lexer) != TOKEN_THEN)
            goto ERROR;
        parser_pop(lexer);
        struct ast_node *response = compound_list(lexer);
        if (response == NULL)
            goto ERROR;
        ast_append(current, response);
        struct ast_node *else_c = else_clause(lexer);
        if (else_c != NULL)
            ast_append(current, else_c);
        return current;
    ERROR:
        ast_free(current);
        return NULL;
    }
    else
        return ast_node_new(AST_EMPTY);
}

struct ast_node *compound_list(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_COMMAND_LIST);
    while (parser_peek(lexer) == TOKEN_EOL)
    {
        parser_pop(lexer);
    }
    struct ast_node *child = and_or(lexer);
    if (current == NULL)
    {
        ast_free(current);
        return NULL;
    }
    ast_append(current, child);
    while (parser_peek(lexer) == TOKEN_SEMICOLON
           || parser_peek(lexer) == TOKEN_EOL)
    {
        parser_pop(lexer);
        while (parser_peek(lexer) == TOKEN_EOL)
        {
            parser_pop(lexer);
        }
        child = and_or(lexer);
        if (child != NULL)
            ast_append(current, child);
    }
    if (parser_peek(lexer) == TOKEN_SEMICOLON)
    {
        parser_pop(lexer);
    }
    while (parser_peek(lexer) == TOKEN_EOL)
    {
        parser_pop(lexer);
    }
    return current;
}
