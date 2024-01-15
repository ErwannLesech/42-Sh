#include <fnmatch.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

void handle_backslash(struct lexer *lexer, bool *is_diactivated, char *word,
                      unsigned word_index)
{
    *is_diactivated = true;
    if (lexer->data[lexer->index] != '\0')
    {
        word[word_index - 1] = lexer->data[lexer->index];
        ++lexer->index;
    }
    else
    {
        word[word_index - 1] = '\0';
    }
}

char *handle_simple_quote(struct lexer *lexer, bool *is_diactivated, char *word,
                          unsigned *word_index)
{
    *is_diactivated = true;
    *word_index -= 1;
    while (lexer->data[lexer->index] != '\'')
    {
        if (lexer->data[lexer->index] == '\0')
        {
            free(word);
            word = NULL;
            return NULL;
        }
        word = realloc(word, sizeof(char) * (*word_index + 1));
        word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
    }

    return word;
}

/**
 * \brief Check if the given word is a variable name.
 * \param lexer The lexer.
 * \param word The word to check.
 */
char *check_variable_name(struct lexer *lexer, char *word, unsigned *word_index)
{
    if (lexer->data[lexer->index] == '?' || lexer->data[lexer->index] == '*'
        || lexer->data[lexer->index] == '@' || lexer->data[lexer->index] == '#'
        || lexer->data[lexer->index] == '$')
    {
        word = realloc(word, sizeof(char) * (*word_index + 1));
        word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
        return word;
    }
    else if (lexer->data[lexer->index] >= '0'
             && lexer->data[lexer->index] <= '9')
    {
        while (lexer->data[lexer->index] >= '0'
               && lexer->data[lexer->index] <= '9')
        {
            word = realloc(word, sizeof(char) * (*word_index + 1));
            word[*word_index] = lexer->data[lexer->index];
            *word_index += 1;
            lexer->index += 1;
        }
        return word;
    }
    else if (lexer->data[lexer->index] == '_'
             || lexer->data[lexer->index] == '-'
             || (lexer->data[lexer->index] >= 'a'
                 && lexer->data[lexer->index] <= 'z')
             || (lexer->data[lexer->index] >= 'A'
                 && lexer->data[lexer->index] <= 'Z'))
    {
        word = realloc(word, sizeof(char) * (*word_index + 1));
        word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
    }
    else
    {
        return NULL;
    }

    while (lexer->data[lexer->index] != ' ' && lexer->data[lexer->index] != '\t'
           && lexer->data[lexer->index] != '\n'
           && lexer->data[lexer->index] != '\0')
    {
        if (lexer->data[lexer->index] == '_' || lexer->data[lexer->index] == '-'
            || (lexer->data[lexer->index] >= 'a'
                && lexer->data[lexer->index] <= 'z')
            || (lexer->data[lexer->index] >= 'A'
                && lexer->data[lexer->index] <= 'Z')
            || (lexer->data[lexer->index] >= '0'
                && lexer->data[lexer->index] <= '9'))
        {
            word = realloc(word, sizeof(char) * (*word_index + 1));
            word[*word_index] = lexer->data[lexer->index];
            *word_index += 1;
            lexer->index += 1;
        }
        else
        {
            return word;
        }
    }

    return word;
}

char *handle_double_quote(struct lexer *lexer, bool *is_diactivated, char *word,
                          unsigned *word_index)
{
    *is_diactivated = true;
    if (lexer->data[lexer->index] == '$')
    {
        word = realloc(word, sizeof(char) * (*word_index + 1));
        word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
        char *word_tmp = check_variable_name(lexer, word, word_index);
        if (word_tmp != NULL)
        {
            word = word_tmp;
        }
        else
        {
            return word;
        }
    }
    while (lexer->data[lexer->index] != '\"' && lexer->data[lexer->index] != '$')
    {
        if (lexer->data[lexer->index] == '\0')
        {
            free(word);
            word = NULL;
            return NULL;
        }
        else if (lexer->data[lexer->index] == '\\')
        {
            lexer->index += 1;
            if (lexer->data[lexer->index] == '\"'
                || lexer->data[lexer->index] == '$'
                || lexer->data[lexer->index] == '\\'
                || lexer->data[lexer->index] == '\n')
            {
                word = realloc(word, sizeof(char) * (*word_index + 1));
                word[*word_index] = lexer->data[lexer->index];
                *word_index += 1;
                lexer->index += 1;
            }
        }
        word = realloc(word, sizeof(char) * (*word_index + 1));
        word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
    }
    if (lexer->data[lexer->index] == '\"')
    {
        lexer->curr_tok.type = TOKEN_EOL;
        ++lexer->index;
    }
    return word;
}

char *handle_comment(struct lexer *lexer, char *word, unsigned *word_index)
{
    // Skip the comment
    ++lexer->index;

    // Find the end of the comment
    while (lexer->data[lexer->index] != '\n'
           && lexer->data[lexer->index] != '\0')
    {
        ++lexer->index;
    }
    word[*word_index] = lexer->data[lexer->index];
    *word_index += 1;
    if (lexer->data[lexer->index] != '\0')
    {
        ++lexer->index;
    }

    // Skip the spaces after the comment
    while (lexer->data[lexer->index] == ' ')
    {
        ++lexer->index;
    }
    return word;
}

char *handle_redir(struct lexer *lexer, unsigned *word_index)
{
    char *redir = malloc(sizeof(char) * 2);
    redir[0] = lexer->data[lexer->index];
    ++lexer->index;
    *word_index += 1;
    if (lexer->data[lexer->index] == '>' || lexer->data[lexer->index] == '&'
        || lexer->data[lexer->index] == '|')
    {
        *word_index += 1;
        redir = realloc(redir, sizeof(char) * 3);
        if (lexer->data[lexer->index - 1] == '<'
            && lexer->data[lexer->index] == '|')
        {
            free(redir);
            return NULL;
        }

        redir[1] = lexer->data[lexer->index];
        ++lexer->index;
    }
    return redir;
}
