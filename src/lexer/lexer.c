/**
 * \file lexer.c
 * \brief Lex the input.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
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

struct lex_match lex_match[] = {
    { "if", TOKEN_IF },         { "then", TOKEN_THEN },
    { "elif", TOKEN_ELIF },     { "else", TOKEN_ELSE },
    { "fi", TOKEN_FI },         { ";", TOKEN_SEMICOLON },
    { "\n", TOKEN_EOL },        { "\0", TOKEN_EOF },

    { "&&", TOKEN_AND },        { "||", TOKEN_OR },
    { "|", TOKEN_PIPE },        { "!", TOKEN_NEGATE },
    { "<", TOKEN_REDIR },       { ">", TOKEN_REDIR },
    { ">>", TOKEN_REDIR },      { "<&", TOKEN_REDIR },
    { ">&", TOKEN_REDIR },      { "done", TOKEN_DONE },
    { ">|", TOKEN_REDIR },      { "<>", TOKEN_REDIR },
    { "(", TOKEN_OPEN_PAR },    { ")", TOKEN_CLOSE_PAR },
    { "{", TOKEN_OPEN_BRACES }, { "}", TOKEN_CLOSE_BRACES }
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

bool first_char_check(struct lexer *lexer, char **current_word,
                      unsigned *word_index)
{
    char *word = *current_word;

    if (lexer->data[lexer->index] == ';' || lexer->data[lexer->index] == '\n')
    {
        word[0] = lexer->data[lexer->index];
        *word_index = 1;
        ++lexer->index;
    }

    // Handle comments return the next word
    else if (lexer->data[lexer->index] == '#')
    {
        word = handle_comment(lexer, word, word_index);
    }

    // Handle redirections return the token
    else if (lexer->data[lexer->index] == '>'
             || lexer->data[lexer->index] == '<')
    {
        free(*current_word);
        word = handle_redir(lexer, word_index);
    }

    // Handle (||, &&, | and &) return the token
    else if (lexer->data[lexer->index] == '|'
             || lexer->data[lexer->index] == '&')
    {
        word[0] = lexer->data[lexer->index];
        *word_index = 1;
        ++lexer->index;

        // Handle ||, &&
        if (lexer->data[lexer->index] == '|'
            || lexer->data[lexer->index] == '&')
        {
            word = realloc(word, sizeof(char) * (*word_index + 1));
            word[*word_index] = lexer->data[lexer->index];
            *word_index = 2;
            ++lexer->index;
        }
    }

    // Handle (, ) return the token
    else if (lexer->data[lexer->index] == '('
             || lexer->data[lexer->index] == ')')
    {
        word[0] = lexer->data[lexer->index];
        *word_index = 1;
        ++lexer->index;
    }
    else
    {
        return false;
    }

    *current_word = word;

    return true;
}

char *end_of_get_word(struct lexer *lexer, char *word, unsigned word_index)
{
    word = append_end_of_word(word, word_index);

    if (is_number(word)
        && (lexer->data[lexer->index] == '>'
            || lexer->data[lexer->index] == '<'))
    {
        lexer->curr_tok.type = TOKEN_IONUMBER;
    }

    // Skip spaces and tabs
    while (lexer->data[lexer->index] == ' '
           || lexer->data[lexer->index] == '\t')
    {
        ++lexer->index;
    }

    return word;
}

char *get_word(struct lexer *lexer, bool *is_diactivated)
{
    char *word = malloc(sizeof(char) * 2);

    unsigned word_index = 0;

    // Word start with ; or \n and return its token
    if (!first_char_check(lexer, &word, &word_index))
    {
        // Handle the word
        while (word_separator_check(lexer))
        {
            word = append_end_of_word(word, word_index);

            // Handle the word assignement if it's contain '=' and it's not the
            // first character

            if (handle_egal(lexer, word, word_index))
            {
                break;
            }

            // Handle the variable
            if (lexer->data[lexer->index] == '$')
            {
                if (word_index != 0
                    && check_variable_name_simulated(lexer->data, lexer->index))
                {
                    break;
                }
                bool is_in_braces = false;
                if (handle_dollar(lexer, &word, &word_index, &is_in_braces))
                {
                    return append_end_of_word(word, word_index);
                }
                else if (is_in_braces)
                {
                    free(word);
                    return NULL;
                }
                else if (lexer->data[lexer->index] == '\0')
                {
                    break;
                }
            }
            // Take next char and put it in the word
            append_char_to_word(lexer, &word, &word_index);

            // Handle the double quote
            if (check_double_quote(lexer))
            {
                // Handle the end of the double quote
                if (word_index > 0 && lexer->data[lexer->index - 1] == '\"')
                {
                    word_index -= 1;
                    lexer->curr_tok.type = TOKEN_DOUBLE_QUOTE;
                }
                else if (word_index > 0
                         && lexer->data[lexer->index - 1] == '\\')
                {
                    word_index -= 1;
                    handle_back_slash_in_double_quote(lexer, word, &word_index);
                    if (lexer->data[lexer->index] == '$')
                    {
                        break;
                    }
                }

                // Handle the double quote
                word = handle_double_quote(lexer, is_diactivated, word,
                                           &word_index);
                // Missing closing double quote
                if (!word)
                {
                    return NULL;
                }

                if (is_sub_or_var_and_double_quote(lexer))
                {
                    return word;
                }
            }

            else if (NULL == hbsq(lexer, word, &word_index, is_diactivated))
            {
                return NULL;
            }
        }
    }

    // End of the word
    return end_of_get_word(lexer, word, word_index);
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    struct token token;

    // Usefull to diactivate the special meaning of the next character when it's
    // a backslash
    bool is_diactivated = false;

    // Skip spaces and tabs at first
    while (lexer->data[lexer->index] == ' '
           || lexer->data[lexer->index] == '\t')
    {
        ++lexer->index;
    }

    // Return the next word
    char *word = get_word(lexer, &is_diactivated);
    // If word is NULL, return an EOF tokens
    if (!word)
    {
        token.type = TOKEN_ERROR;
        token.data = "get_word - Missing closing quote.";
        return token;
    }

    // Check if the word is a special case
    token = check_special_cases(lexer, word, token);
    if (token.type != TOKEN_ERROR)
    {
        return token;
    }

    // Check if is in the lex_match table
    for (unsigned i = 0; i < sizeof(lex_match) / sizeof(*lex_match); ++i)
    {
        if (fnmatch(lex_match[i].str, word, 0) == 0 && !is_diactivated)
        {
            if (lex_match[i].type == TOKEN_EOF
                && (lexer->curr_tok.type == TOKEN_DOUBLE_QUOTE
                    || lexer->curr_tok.type == TOKEN_WORD_DOUBLE_QUOTE
                    || lexer->curr_tok.type == TOKEN_VARIABLE_AND_DOUBLE_QUOTE))
            {
                token.type = TOKEN_ERROR;
                token.data = "parse_input_for_tok - Missing closing quote.";
                free(word);
                return token;
            }
            token.type = lex_match[i].type;
            token.data = word;
            return token;
        }
    }

    // Check if it's a variable value
    if (lexer->curr_tok.type == TOKEN_VARIABLE_VALUE)
    {
        lexer->curr_tok.type = TOKEN_EOL;
    }
    // Else it's a word
    token.type = lexer->curr_tok.type == TOKEN_DOUBLE_QUOTE
        ? TOKEN_WORD_DOUBLE_QUOTE
        : TOKEN_WORD;
    token.data = word;
    return token;
}

struct token lexer_peek(struct lexer *lexer)
{
    size_t save_index = lexer->index;
    struct token save_token = lexer->curr_tok;
    if (lexer->curr_tok.type == TOKEN_EOF)
    {
        struct token token;
        token.type = TOKEN_EOF;
        token.data = "\0";
        return token;
    }
    struct token token = parse_input_for_tok(lexer);
    lexer->index = save_index;
    lexer->curr_tok = save_token;
    /*free(token.data);
    token.data = NULL;*/
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
    if (token.type == TOKEN_EOF)
    {
        lexer->curr_tok = token;
    }
    return token;
}
