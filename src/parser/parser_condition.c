#include "parser.h"

struct ast_node *shell_command(struct lexer *lexer)
{
    return rule_if(lexer);
}

struct ast_node *rule_if(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_CONDITION);
    if (lexer_peek(lexer).type == TOKEN_IF)
    {
        lexer_pop(lexer);
        struct ast_node *condition = compound_list(lexer);
        if (condition == NULL)
            goto ERROR;
        ast_append(current, condition);
        if (lexer_peek(lexer).type != TOKEN_THEN)
            goto ERROR;
        lexer_pop(lexer);
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
        if (lexer_peek(lexer).type != TOKEN_FI)
            goto ERROR;
        lexer_pop(lexer);
        return current;
    }
ERROR:
    ast_free(current);
    return NULL;
}

struct ast_node *else_clause(struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_ELSE)
    {
        lexer_pop(lexer);
        struct ast_node *current = compound_list(lexer);
        if (current == NULL)
            goto ERROR;
        return current;
    }
    else if (lexer_peek(lexer).type == TOKEN_ELIF)
    {
        lexer_pop(lexer);
        struct ast_node *current = ast_node_new(AST_CONDITION);
        struct ast_node *condition = compound_list(lexer);
        if (condition == NULL)
            goto ERROR;
        ast_append(current, condition);
        if (lexer_peek(lexer).type != TOKEN_THEN)
            goto ERROR;
        lexer_pop(lexer);
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
    while (lexer_peek(lexer).type == TOKEN_EOL)
    {
        lexer_pop(lexer);
    }
    struct ast_node *child = and_or(lexer);
    if (current == NULL)
    {
        ast_free(current);
        return NULL;
    }
    ast_append(current, child);
    while (lexer_peek(lexer).type == TOKEN_SEMICOLON
           || lexer_peek(lexer).type == TOKEN_EOL)
    {
        lexer_pop(lexer);
        while (lexer_peek(lexer).type == TOKEN_EOL)
        {
            lexer_pop(lexer);
        }
        child = and_or(lexer);
        if (child != NULL)
            ast_append(current, child);
    }
    if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
    {
        lexer_pop(lexer);
    }
    while (lexer_peek(lexer).type == TOKEN_EOL)
    {
        lexer_pop(lexer);
    }
    return current;
}
