#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <stddef.h>
#include <stdbool.h>

struct lexer
{
    const char *data;
    size_t index;
    struct token curr_tok;
};

struct lex_match
{
    const char *str;
    enum token_type type;
};


/**
 * \brief Create a new lexer from the given input string.
*/
struct lexer *lexer_new(const char *input);

/**
 * \brief Free the given lexer.
 */
void lexer_free(struct lexer *lexer);

/**
 * \brief Returns a token from the input string.
 */
struct token parse_input_for_tok(struct lexer *lexer);

/**
 * \brief Returns the next token, but doesn't move forward in the stream.
 */
struct token lexer_peek(struct lexer *lexer);

/**
 * \brief Returns the next token, and removes it from the stream.
 */
struct token lexer_pop(struct lexer *lexer);

/**
 * \brief Returns the next word in the input string.
 */
char *get_word(struct lexer *lexer, bool *is_diactivated);

#endif /* !LEXER_H */