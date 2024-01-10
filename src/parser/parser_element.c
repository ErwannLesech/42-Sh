#include "parser_element.h"

struct ast_node *input(struct lexer *lexer)
{
    struct ast_node *node = list(lexer);
    if (lexer_peek(lexer).type == TOKEN_EOL 
    || lexer_peek(lexer).type == TOKEN_EOF)
    {
        return node;
    }
    return NULL;
}

struct ast_node *list(struct lexer *lexer)
{
    struct ast_node *current = ast_node_new(AST_COMMAND_LIST);
    struct ast_node *child = and_or(lexer);
    if (child != NULL)
    {
        ast_append(current, child);
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        {
            lexer_pop(lexer);
            child = and_or(lexer);
            if (child == NULL)
            {
                free(current);
                return NULL;
            }
            ast_append(current, child);
        }
        if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
            lexer_pop(lexer);
        return current;
    }
    free(current);
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
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        struct ast_node *new = ast_node_word(lexer_peek(lexer).data);
        lexer_pop(lexer);
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
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        struct ast_node *curr = ast_node_word(lexer_peek(lexer).data);
        lexer_pop(lexer);
        return curr;
    }
    return NULL;
}
