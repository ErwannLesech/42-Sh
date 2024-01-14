/**
 * \file lexer.c
 * \brief Lex the input.
 * \author Erwann Lesech, Valentin Gibert, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include "lexer.h"

#include <err.h>
#include <fnmatch.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lex_match lex_match[25] = { { "if", TOKEN_IF },
                                   { "then", TOKEN_THEN },
                                   { "elif", TOKEN_ELIF },
                                   { "else", TOKEN_ELSE },
                                   { "fi", TOKEN_FI },
                                   { ";", TOKEN_SEMICOLON },
                                   { "\n", TOKEN_EOL },
                                   { "\0", TOKEN_EOF },

                                   { "&&", TOKEN_AND },
                                   { "||", TOKEN_OR },
                                   { "|", TOKEN_PIPE },
                                   { "!", TOKEN_NEGATE },
                                   { "[0-9]*<", TOKEN_INPUT_REDIR },
                                   { "[0-9]*>", TOKEN_OUTPUT_REDIR },
                                   { "[0-9]*>>", TOKEN_APPEND },
                                   { "[0-9]*<&", TOKEN_DUP_INPUT },
                                   { "[0-9]*>&", TOKEN_DUP_INPUT_OUTPUT },
                                   { "[0-9]*>|", TOKEN_NOCLOBBER },
                                   { "[0-9]*<>", TOKEN_DUP_INPUT_OUTPUT },
                                   { "while", TOKEN_WHILE },
                                   { "until", TOKEN_UNTIL },
                                   { "for", TOKEN_FOR },
                                   { "do", TOKEN_DO },
                                   { "done", TOKEN_DONE },
                                   { "$*", TOKEN_VARIABLE } };

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
           && lexer->data[lexer->index] != '\n'
           && lexer->data[lexer->index] != '\t')
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

    while (lexer->data[lexer->index] == ' '
           || lexer->data[lexer->index] == '\t')
    {
        ++lexer->index;
    }

    return word;
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    struct token token;

    bool is_diactivated = false;

    while (lexer->data[lexer->index] == ' '
           || lexer->data[lexer->index] == '\t')
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
        if (fnmatch(lex_match[i].str, word, 0) == 0 && !is_diactivated)
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
