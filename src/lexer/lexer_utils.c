#include <stdlib.h>

#include "lexer.h"

bool handle_backslash(struct lexer *lexer, bool *is_diactivated, char *word,
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
        return false;
    }

    return true;
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

char *handle_comment(struct lexer *lexer, char *word, unsigned word_index)
{
    // Skip the comment
    ++lexer->index;

    // Find the end of the comment
    while (lexer->data[lexer->index] != '\n'
           && lexer->data[lexer->index] != '\0')
    {
        ++lexer->index;
    }
    word[word_index] = lexer->data[lexer->index];
    ++lexer->index;
    // If the comment isn't the last thing in the string, we need to add a '\0'
    // at the end of the word.
    if (word[word_index] != '\0')
    {
        word[word_index + 1] = '\0';
    }

    // Skip the spaces after the comment
    while (lexer->data[lexer->index] == ' ')
    {
        ++lexer->index;
    }
    return word;
}