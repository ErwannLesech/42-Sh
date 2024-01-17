/**
 * \file token.h
 * \brief Header of the token structure
 * \authors Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef TOKEN_H
#define TOKEN_H

/**
 * \enum token_type
 * \brief Enum of all the possible token types
 */
enum token_type
{
    // Step 1
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_WORD, // Any word
    TOKEN_EOL, // End of line ('\n')
    TOKEN_EOF, // End of file
    TOKEN_ERROR, // Error

    // Step 2
    TOKEN_DONE, // done
    TOKEN_AND, // &&
    TOKEN_OR, // ||
    TOKEN_PIPE, // |
    TOKEN_NEGATE, // \!
    TOKEN_IONUMBER, // [0-9]+
    TOKEN_REDIR, // >, <, >>, >&, <&, >|, <>
    TOKEN_DOUBLE_QUOTE, // "
    TOKEN_WORD_ASSIGNMENT, // variable=
    TOKEN_VARIABLE, // $variable

    // Internal values for lexer
    TOKEN_VARIABLE_VALUE, // =value
    TOKEN_VARIABLE_AND_DOUBLE_QUOTE // $variable"
};

/**
 * \struct token
 * \brief Structure representing a token
 */
struct token
{
    enum token_type type;
    char *data;
};

#endif /* !TOKEN_H */
