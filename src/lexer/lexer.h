/**
 * \file lexer.h
 * \brief Header of the lexer
 * \authors Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

#include "token.h"
#include "../options/options.h"

/**
 * \struct lexer
 * \brief Structure representing a lexer
 */
struct lexer
{
    const char *data;
    size_t index;
    struct token curr_tok;
};

/**
 * \struct lex_match
 * \brief Structure representing a match between a string and a token type
 */
struct lex_match
{
    const char *str;
    enum token_type type;
};

/**
 * \brief Create a new lexer from the given input string.
 * \param input The input string.
 * \return A pointer to the lexer.
 */
struct lexer *lexer_new(const char *input);

/**
 * \brief Free the given lexer.
 * \param lexer The lexer to free.
 */
void lexer_free(struct lexer *lexer);

/**
 * \brief Free the given token.
 * \param token The token to free.
 */
void token_free(struct token token);

/**
 * \brief Get the next token from the lexer.
 * \param lexer The lexer.
 * \return The next token.
 */
struct token parse_input_for_tok(struct lexer *lexer);

/**
 * \brief Get the next token, but doesn't move forward in the stream.*
 * \param lexer The lexer.
 * \return The next token.
 */
struct token lexer_peek(struct lexer *lexer);

/**
 * \brief Get the next token, and removes it from the stream.
 * \param lexer The lexer.
 * \return The next token.
 */
struct token lexer_pop(struct lexer *lexer);

/**
 * \brief Handle the backslash character.
 *
 * \return false if it's the end of the string, true otherwise.
 */
void handle_backslash(struct lexer *lexer, bool *is_diactivated, char *word,
                      unsigned word_index);

/**
 * \brief Handle the simple quote character.
 *
 * \return false if a closing quote was not found, true otherwise.
 */
char *handle_simple_quote(struct lexer *lexer, bool *is_diactivated, char *word,
                          unsigned *word_index);

/**
 * \brief Check if the word is a variable assignement.
 *
 * \param lexer The lexer.
 * \param word The word to check.
 * \param word_index The index of the word.
 *
 * \return if the word is a variable assignement.
 */
bool check_variable_assignement(char *word);

/**
 * \brief Check if the given word is a variable name.
 * \param lexer The lexer.
 * \param word The word to check.
 * \param word_index The index of the word.
 *
 * \return if the word is a variable name.
 */
bool check_variable_name(struct lexer *lexer, char **word, unsigned *word_index,
                         bool *is_in_braces);

/**
 * \brief Handle the dollar character.
 * \param lexer The lexer.
 * \return The next word.
 */
bool handle_dollar(struct lexer *lexer, char **word, unsigned *word_index,
                   bool *is_in_braces);

/**
 * \brief Handle the double quote character.
 * \param lexer The lexer.
 * \return The next word.
 */
char *handle_double_quote(struct lexer *lexer, bool *is_diactivated, char *word,
                          unsigned *word_index);

/**
 * \brief Handle the comment character.
 *
 * \return The next word.
 */
char *handle_comment(struct lexer *lexer, char *word, unsigned *word_index);

/**
 * \brief Handle the redirection character.
 * \param lexer The lexer.
 * \return The next redirection word.
 */
char *handle_redir(struct lexer *lexer, unsigned *word_index);

/**
 * \brief Returns the next word in the input string.
 * \param lexer The lexer.
 * \param is_diactivated A pointer to a boolean that will be set to true if the
 * word is diactivated. \return The next word.
 */
char *get_word(struct lexer *lexer, bool *is_diactivated);

#endif /* !LEXER_H */
