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

bool check_variable_assignement(char *word)
{
    size_t i = 0;
    if (word[i] == '_' || word[i] == '-' || (word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
    {
        i++;
    }
    else
    {
        return false;
    }
    while (word[i] != ' ' && word[i] != '\t'
           && word[i] != '\n'
           && word[i] != '\0')
    {
        if (word[i] == '_' || word[i] == '-'
            || (word[i] >= 'a'
                && word[i] <= 'z')
            || (word[i] >= 'A'
                && word[i] <= 'Z')
            || (word[i] >= '0'
                && word[i] <= '9'))
        {
            i++;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool check_variable_name(struct lexer *lexer, char **word, unsigned *word_index, bool *is_in_braces)
{
    char *curr_word = *word;
    *is_in_braces = false;
    // Handle variable in double quote
    if (lexer->curr_tok.type == TOKEN_DOUBLE_QUOTE)
    {
        lexer->curr_tok.type = TOKEN_VARIABLE_AND_DOUBLE_QUOTE;
    }
    else
    {
        lexer->curr_tok.type = TOKEN_VARIABLE;
    }

    // Check if it's a special variable (like $?, $*, $@, $# or $$)
    if (lexer->data[lexer->index] == '?' || lexer->data[lexer->index] == '*'
        || lexer->data[lexer->index] == '@' || lexer->data[lexer->index] == '#'
        || lexer->data[lexer->index] == '$')
    {
        curr_word = realloc(curr_word, sizeof(char) * (*word_index + 1));
        curr_word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
        *word = curr_word;
        return true;
    }

    // Chech if it's a special variable (like $n)
    else if (lexer->data[lexer->index] >= '0'
             && lexer->data[lexer->index] <= '9')
    {
        while (lexer->data[lexer->index] >= '0'
               && lexer->data[lexer->index] <= '9')
        {
            curr_word = realloc(curr_word, sizeof(char) * (*word_index + 1));
            curr_word[*word_index] = lexer->data[lexer->index];
            *word_index += 1;
            lexer->index += 1;
        }
        *word = curr_word;
        return true;
    }

    else if (lexer->data[lexer->index] == '{')
    {
        lexer->index += 1;
        *is_in_braces = true;
    }

    // Classic variable name
    else if (lexer->data[lexer->index] == '_'
             || lexer->data[lexer->index] == '-'
             || (lexer->data[lexer->index] >= 'a'
                 && lexer->data[lexer->index] <= 'z')
             || (lexer->data[lexer->index] >= 'A'
                 && lexer->data[lexer->index] <= 'Z'))
    {
        curr_word = realloc(curr_word, sizeof(char) * (*word_index + 1));
        curr_word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
    }
    // Not a valid variable name
    else
    {
        if (lexer->curr_tok.type != TOKEN_DOUBLE_QUOTE)
        {
            lexer->curr_tok.type = TOKEN_WORD;
        }
        return false;
    }

    // Check the rest of the variable name break
    while (lexer->data[lexer->index] == '_' || lexer->data[lexer->index] == '-'
            || (lexer->data[lexer->index] >= 'a'
                && lexer->data[lexer->index] <= 'z')
            || (lexer->data[lexer->index] >= 'A'
                && lexer->data[lexer->index] <= 'Z')
            || (lexer->data[lexer->index] >= '0'
                && lexer->data[lexer->index] <= '9'))
    {
        curr_word = realloc(curr_word, sizeof(char) * (*word_index + 1));
        curr_word[*word_index] = lexer->data[lexer->index];
        *word_index += 1;
        lexer->index += 1;
    }

    if (*is_in_braces)
    {
        if (lexer->data[lexer->index] == '}')
        {
            lexer->index += 1;
            *is_in_braces = false;
        }
    }
    *word = curr_word;
    return true;
}

bool handle_dollar(struct lexer *lexer, char **word,
                          unsigned *word_index, bool *is_in_braces)
{
    char *curr_word = *word;
    // Add the dollar to the word
    curr_word = realloc(curr_word, sizeof(char) * (*word_index + 1));
    curr_word[*word_index] = lexer->data[lexer->index];
    *word_index += 1;
    lexer->index += 1;
    *word = curr_word;

    // Check if the name of the variable is correct
    return check_variable_name(lexer, word, word_index, is_in_braces);

}

char *handle_double_quote(struct lexer *lexer, bool *is_diactivated, char *word,
                          unsigned *word_index)
{
    *is_diactivated = true;
    bool is_in_braces = false;
    // Check if a the first word is a variable
    if (lexer->data[lexer->index] == '$')
    {
        if (handle_dollar(lexer, &word, word_index, &is_in_braces))
        {
            word = realloc(word, sizeof(char) * (*word_index + 1));
            word[*word_index] = '\0';
            return word;
        }
        else if (is_in_braces)
        {
            free(word);
            return NULL;
        }
    }

    // While it's different from a double quote or a variable
    while (lexer->data[lexer->index] != '\"' && lexer->data[lexer->index] != '$')
    {
        // Missing closing double quote
        if (lexer->data[lexer->index] == '\0')
        {
            free(word);
            word = NULL;
            return NULL;
        }
        // Handle the backslash if the back slash is alone we need to add it to the word
        else if (lexer->data[lexer->index] == '\\')
        {
            lexer->index += 1;
            word = realloc(word, sizeof(char) * (*word_index + 1));
            if (lexer->data[lexer->index] == '\"'
                || lexer->data[lexer->index] == '$'
                || lexer->data[lexer->index] == '\\'
                || lexer->data[lexer->index] == '\n')
            {
                word[*word_index] = lexer->data[lexer->index];
                lexer->index += 1;
            }
            else
            {
                word[*word_index] = '\\';
            }
            *word_index += 1;
        }
        else
        {
            // Add the character to the word
            word = realloc(word, sizeof(char) * (*word_index + 1));
            word[*word_index] = lexer->data[lexer->index];
            *word_index += 1;
            lexer->index += 1;
        }
    }

    // If 
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
    else
    {
        return word;
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
