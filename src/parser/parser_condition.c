/**
 * \file parser_condition.c
 * \brief Parse the condition rule.
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
    struct ast_node *current = NULL;

    if (parser_peek(lexer) == TOKEN_ELSE)
    {
        parser_pop(lexer);
        current = compound_list(lexer);
        if (current == NULL)
            goto ERROR;
        return current;
    }
    else if (parser_peek(lexer) == TOKEN_ELIF)
    {
        parser_pop(lexer);
        current = ast_node_new(AST_CONDITION);
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
    if (child == NULL)
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

struct ast_node *rule_while(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_WHILE);
    if (parser_peek(lexer) == TOKEN_WORD)
    {
        char *value = lexer_peek(lexer).data;
        if (strcmp(value, "while") == 0)
        {
            free(value);
            parser_pop(lexer);
            struct ast_node *condition = compound_list(lexer);
            if (condition == NULL)
            {
                goto ERROR;
            }
            ast_append(current, condition);
            value = lexer_peek(lexer).data;
            if (strcmp(value, "do") == 0)
            {
                free(value);
                parser_pop(lexer);
                struct ast_node *response = compound_list(lexer);
                if (response == NULL)
                    goto ERROR;
                ast_append(current, response);
                value = lexer_peek(lexer).data;
                if (strcmp(value, "done") == 0)
                {
                    free(value);
                    parser_pop(lexer);
                    return current;
                }
            }
        }
        free(value);
    }
ERROR:
    ast_free(current);
    return NULL;
}

struct ast_node *rule_until(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_UNTIL);
    if (parser_peek(lexer) == TOKEN_WORD)
    {
        char *value = lexer_peek(lexer).data;
        if (strcmp(value, "until") == 0)
        {
            free(value);
            parser_pop(lexer);
            struct ast_node *condition = compound_list(lexer);
            if (condition == NULL)
            {
                goto ERROR;
            }
            ast_append(current, condition);
            value = lexer_peek(lexer).data;
            if (strcmp(value, "do") == 0)
            {
                free(value);
                parser_pop(lexer);
                struct ast_node *response = compound_list(lexer);
                if (response == NULL)
                    goto ERROR;
                ast_append(current, response);
                value = lexer_peek(lexer).data;
                if (strcmp(value, "done") == 0)
                {
                    free(value);
                    parser_pop(lexer);
                    return current;
                }
            }
        }
        free(value);
    }
ERROR:
    ast_free(current);
    return NULL;
}

struct ast_node *rule_for(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_FOR);

    if (parser_peek(lexer) == TOKEN_WORD)
    {
        char *value = lexer_peek(lexer).data;
        if (strcmp(value, "for") == 0)
        {
            free(value);
            parser_pop(lexer);
            if (parser_peek(lexer) != TOKEN_WORD)
            {
                goto ERROR;
            }
            value = lexer_peek(lexer).data;
            struct ast_node *var = ast_node_new(AST_WORD);
            var->value = value;
            ast_append(current, var);
            parser_pop(lexer);
            if (parser_peek(lexer) == TOKEN_SEMICOLON)
            {
                goto END;
            }

            while (parser_peek(lexer) == TOKEN_EOL)
            {
                parser_pop(lexer);
            }

            value = lexer_peek(lexer).data;
            if (strcmp(value, "in") == 0)
            {
                free(value);
                parser_pop(lexer);
                value = lexer_peek(lexer).data;
                while ((parser_peek(lexer) == TOKEN_WORD
                        || parser_peek(lexer) == TOKEN_VARIABLE)
                       && strcmp(value, "do") != 0)
                {
                    struct ast_node *condition = ast_node_new(AST_WORD);
                    condition->value = value;
                    ast_append(current, condition);
                    parser_pop(lexer);
                    value = lexer_peek(lexer).data;
                }
                free(value);
                if (parser_peek(lexer) != TOKEN_SEMICOLON
                    && parser_peek(lexer) != TOKEN_EOL)
                {
                    goto ERROR;
                }
                parser_pop(lexer);
            }
            else
            {
                goto ERROR;
            }

        END:
            while (parser_peek(lexer) == TOKEN_EOL)
            {
                parser_pop(lexer);
            }

            value = lexer_peek(lexer).data;
            if (strcmp(value, "do") == 0)
            {
                free(value);
                parser_pop(lexer);
                struct ast_node *response = compound_list(lexer);
                if (response == NULL)
                    goto ERROR;
                ast_append(current, response);
                value = lexer_peek(lexer).data;

                if (strcmp(value, "done") == 0)
                {
                    free(value);
                    parser_pop(lexer);
                    return current;
                }
            }
        }
        // free(value);
    }
ERROR:
    ast_free(current);
    return NULL;
}
