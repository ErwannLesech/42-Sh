#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

struct lexer
{
    char *data;
    size_t index;
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


#endif /* !LEXER_H */