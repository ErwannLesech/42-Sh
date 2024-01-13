/**
 * \file lexer.c
 * \brief Lex the input.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "lexer.h"

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lex_match lex_match[8] = {
    { "if", TOKEN_IF },     { "then", TOKEN_THEN }, { "elif", TOKEN_ELIF },
    { "else", TOKEN_ELSE }, { "fi", TOKEN_FI },     { ";", TOKEN_SEMICOLON },
    { "\n", TOKEN_EOL },    { "\0", TOKEN_EOF }
};

struct lexer *lexer_new(const char *input)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));

    if (!lexer)
    {
        errx(1, "lexer_new - Failed to allocate memory for lexer.");
    }

    lexer->data = input;
    lexer->index = 0;
    lexer->curr_tok.type = TOKEN_EOL;
    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    free(lexer);
}

void token_free(struct token token)
{
    free(token.data);
}

/**
 * \brief Handle the backslash character.
 *
 * \return false if it's the end of the string, true otherwise.
 */
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

/**
 * \brief Handle the simple quote character.
 *
 * \return false if a closing quote was not found, true otherwise.
 */
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

char *get_word(struct lexer *lexer, bool *is_diactivated)
{
    char *word = malloc(sizeof(char) * 2);
    unsigned word_index = 0;
    if (lexer->data[lexer->index] == '\0')
    {
        ++lexer->index;
        word[0] = '\0';
        return word;
    }
    if (lexer->data[lexer->index] == ';' || lexer->data[lexer->index] == '\n')
    {
        word[0] = lexer->data[lexer->index];
        word[1] = '\0';
        ++lexer->index;
        if (lexer->data[lexer->index] == ' ')
        {
            ++lexer->index;
        }
        return word;
    }
    if (lexer->data[lexer->index] == '#')
    {
        return handle_comment(lexer, word, 0);
    }
    while (lexer->data[lexer->index] != ' ' && lexer->data[lexer->index] != '\0'
           && lexer->data[lexer->index] != ';'
           && lexer->data[lexer->index] != '\n')
    {
        word = realloc(word, sizeof(char) * (word_index + 1));
        word[word_index] = lexer->data[lexer->index];
        ++word_index;
        ++lexer->index;
        if (lexer->data[lexer->index - 1] == '\\')
        {
            if (!handle_backslash(lexer, is_diactivated, word, word_index))
            {
                return word;
            }
        }
        else if (lexer->data[lexer->index - 1] == '\'')
        {
            word =
                handle_simple_quote(lexer, is_diactivated, word, &word_index);
            if (!word)
            {
                return NULL;
            }
            lexer->index += 1;
        }
    }
    word = realloc(word, sizeof(char) * (word_index + 1));
    word[word_index] = '\0';

    while (lexer->data[lexer->index] == ' ')
    {
        ++lexer->index;
    }

    return word;
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    struct token token;

    bool is_diactivated = false;

    while (lexer->data[lexer->index] == ' ' || lexer->data[lexer->index] == '\t')
    {
        ++lexer->index;
    }

    char *word = get_word(lexer, &is_diactivated);

    if (!word)
    {
        token.type = TOKEN_ERROR;
        token.data = "get_word - Missing closing quote.";
        return token;
    }

    for (unsigned i = 0; i < sizeof(lex_match) / sizeof(*lex_match); ++i)
    {
        if (!strcmp(word, lex_match[i].str) && !is_diactivated)
        {
            token.type = lex_match[i].type;
            token.data = word;
            return token;
        }
    }

    token.type = TOKEN_WORD;
    token.data = word;
    return token;
}

struct token lexer_peek(struct lexer *lexer)
{
    size_t save_index = lexer->index;
    if (lexer->curr_tok.type == TOKEN_EOF)
    {
        struct token token;
        token.type = TOKEN_EOF;
        token.data = "\0";
        return token;
    }
    struct token token = parse_input_for_tok(lexer);
    lexer->index = save_index;
    return token;
}

struct token lexer_pop(struct lexer *lexer)
{
    if (lexer->curr_tok.type == TOKEN_EOF)
    {
        struct token token;
        token.type = TOKEN_EOF;
        token.data = "\0";
        return token;
    }
    struct token token = parse_input_for_tok(lexer);
    lexer->curr_tok = token;
    return token;
}
