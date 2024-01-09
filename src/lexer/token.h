#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_SIMPLE_QUOTE,
    TOKEN_WORD, // Any word 
    TOKEN_EOL, // End of line ('\n')
    TOKEN_EOF // End of file
};

struct token 
{
    enum token_type type;
    char *data;
};

#endif /* !TOKEN_H */
