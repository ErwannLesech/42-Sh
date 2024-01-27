#include "lexer.h"

struct token check_special_cases(struct lexer *lexer, char *word,
                                 struct token token)
{
    // Check if the word is a word_assignement (contains a '=') and if it's a
    // variable name is valid
    if (lexer->curr_tok.type == TOKEN_WORD_ASSIGNMENT
        && check_variable_assignement(word))
    {
        token.type = TOKEN_WORD_ASSIGNMENT;
        token.data = word;
        // Usefull to have the next word token
        lexer->curr_tok.type = TOKEN_VARIABLE_VALUE;
        return token;
    }

    if (lexer->curr_tok.type == TOKEN_IONUMBER)
    {
        token.type = TOKEN_IONUMBER;
        token.data = word;
        lexer->curr_tok.type = TOKEN_EOL;
        return token;
    }

    // Check if the word is a variable name
    if (lexer->curr_tok.type == TOKEN_VARIABLE
        || lexer->curr_tok.type == TOKEN_VARIABLE_AND_DOUBLE_QUOTE)
    {
        token.type = TOKEN_VARIABLE;
        token.data = word;
        if (lexer->curr_tok.type == TOKEN_VARIABLE_AND_DOUBLE_QUOTE)
        {
            lexer->curr_tok.type = TOKEN_DOUBLE_QUOTE;
        }
        else
        {
            lexer->curr_tok.type = TOKEN_EOL;
        }
        return token;
    }

    if (lexer->curr_tok.type == TOKEN_SUBSTITUTION
        || lexer->curr_tok.type == TOKEN_SUB_AND_DOUBLE_QUOTE)
    {
        token.type = TOKEN_SUBSTITUTION;
        token.data = word;
        lexer->curr_tok.type = TOKEN_EOL;
        return token;
    }
    token.type = TOKEN_ERROR;
    return token;
}

bool word_separator_check(struct lexer *lexer)
{
    if (lexer->data[lexer->index] != ' ' && lexer->data[lexer->index] != '\0'
        && lexer->data[lexer->index] != ';' && lexer->data[lexer->index] != '\n'
        && lexer->data[lexer->index] != '\t' && lexer->data[lexer->index] != '>'
        && lexer->data[lexer->index] != '<' && lexer->data[lexer->index] != '|'
        && lexer->data[lexer->index] != '&' && lexer->data[lexer->index] != '('
        && lexer->data[lexer->index] != ')')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool handle_egal(struct lexer *lexer, char *word, unsigned word_index)
{
    if (lexer->data[lexer->index] == '=' && word_index > 0
        && lexer->curr_tok.type != TOKEN_DOUBLE_QUOTE
        && lexer->curr_tok.type != TOKEN_VARIABLE_VALUE
        && check_variable_assignement(word))
    {
        lexer->curr_tok.type = TOKEN_WORD_ASSIGNMENT;
        return true;
    }

    else if (lexer->data[lexer->index] == '=' && word_index == 0
             && lexer->curr_tok.type == TOKEN_VARIABLE_VALUE)
    {
        lexer->index += 1;
    }

    return false;
}

char *hbsq(struct lexer *lexer, char *word, unsigned *word_index,
           bool *is_diactivated)
{
    if (lexer->data[lexer->index - 1] == '\\')
    {
        handle_backslash(lexer, is_diactivated, word, *word_index);
    }

    // Handle simple quote
    else if (lexer->data[lexer->index - 1] == '\'')
    {
        word = handle_simple_quote(lexer, is_diactivated, word, word_index);

        // Missing closing simple quote
        if (!word)
        {
            return NULL;
        }
        lexer->index += 1;
    }

    return word;
}

bool check_double_quote(struct lexer *lexer)
{
    // Handle the double quote
    if (lexer->data[lexer->index - 1] == '\"'
        || lexer->curr_tok.type == TOKEN_DOUBLE_QUOTE
        || lexer->curr_tok.type == TOKEN_VARIABLE_AND_DOUBLE_QUOTE)
    {
        return true;
    }

    return false;
}

bool is_sub_or_var_and_double_quote(struct lexer *lexer)
{
    if (lexer->curr_tok.type == TOKEN_VARIABLE_AND_DOUBLE_QUOTE
        || lexer->curr_tok.type == TOKEN_SUB_AND_DOUBLE_QUOTE)
    {
        return true;
    }

    return false;
}
