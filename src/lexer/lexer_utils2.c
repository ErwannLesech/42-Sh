#include "lexer.h"

void print_token(struct token token)
{
    char *tokens[] = { // Step1
                       [TOKEN_IF] = "TOKEN_IF",
                       [TOKEN_THEN] = "TOKEN_THEN",
                       [TOKEN_ELIF] = "TOKEN_ELIF",
                       [TOKEN_ELSE] = "TOKEN_ELSE",
                       [TOKEN_FI] = "TOKEN_FI",
                       [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
                       [TOKEN_WORD] = "TOKEN_WORD",
                       [TOKEN_EOL] = "TOKEN_EOL",
                       [TOKEN_EOF] = "TOKEN_EOF",
                       [TOKEN_ERROR] = "TOKEN_ERROR",
                       // Step 2
                       [TOKEN_DONE] = "TOKEN_DONE",
                       [TOKEN_AND] = "TOKEN_AND",
                       [TOKEN_OR] = "TOKEN_OR",
                       [TOKEN_PIPE] = "TOKEN_PIPE",
                       [TOKEN_NEGATE] = "TOKEN_NEGATE",
                       [TOKEN_IONUMBER] = "TOKEN_IONUMBER",
                       [TOKEN_REDIR] = "TOKEN_REDIR",
                       [TOKEN_DOUBLE_QUOTE] = "TOKEN_DOUBLE_QUOTE",
                       [TOKEN_WORD_DOUBLE_QUOTE] = "TOKEN_WORD_DOUBLE_QUOTE",
                       [TOKEN_WORD_ASSIGNMENT] = "TOKEN_WORD_ASSIGNMENT",
                       [TOKEN_VARIABLE] = "TOKEN_VARIABLE",

                       // Internal values for lexer
                       [TOKEN_VARIABLE_VALUE] = "TOKEN_VARIABLE_VALUE",
                       [TOKEN_VARIABLE_AND_DOUBLE_QUOTE] =
                           "TOKEN_VARIABLE_AND_DOUBLE_QUOTE"
    };
    printf("Token: %s\n", tokens[token.type]);
}

bool check_variable_name_simulated(const char *data, int index)
{
    bool is_in_braces = false;

    index += 1;

    // Check if it's a special variable (like $?, $*, $@, $# or $$)
    if (data[index] == '?' || data[index] == '*' || data[index] == '@'
        || data[index] == '#' || data[index] == '$')
    {
        return true;
    }

    // Chech if it's a special variable (like $n)
    else if (data[index] >= '0' && data[index] <= '9')
    {
        return true;
    }

    else if (data[index] == '{')
    {
        index += 1;
        is_in_braces = true;
    }

    // Classic variable name
    else if (data[index] == '_' || data[index] == '-'
             || (data[index] >= 'a' && data[index] <= 'z')
             || (data[index] >= 'A' && data[index] <= 'Z'))
    {
        index += 1;
    }
    // Not a valid variable name
    else
    {
        return false;
    }

    // Check the rest of the variable name break
    while (data[index] == '_' || data[index] == '-'
           || (data[index] >= 'a' && data[index] <= 'z')
           || (data[index] >= 'A' && data[index] <= 'Z')
           || (data[index] >= '0' && data[index] <= '9'))
    {
        index += 1;
    }

    if (is_in_braces)
    {
        if (data[index] == '}')
        {
            index += 1;
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool check_subshell(struct lexer *lexer, char **word, unsigned *word_index)
{   
    char *subshell = *word;
    int subshell_depth = 0;

    if (lexer->data[lexer->index] == '(')
    {
        subshell_depth += 1; 
        lexer->index += 1;
    }
    else
    {
        return false;
    }

    while (lexer->data[lexer->index] != '\0')
    {
        if (lexer->data[lexer->index] == '(')
        {
            subshell_depth += 1; 
            lexer->index += 1;
        }
        else if (lexer->data[lexer->index] == ')')
        {
            subshell_depth -= 1; 
            lexer->index += 1;
            if (subshell_depth == 0)
            {
                return true;
            }
        }
        else
        {
            subshell = realloc(subshell, sizeof(char) * (*word_index + 1));
            subshell[*word_index] = lexer->data[lexer->index];
            *word_index += 1;
            lexer->index += 1;
        }
    }

    return false;
}


void handle_back_slash_in_double_quote(struct lexer *lexer, char *word,
                                       unsigned *word_index)
{
    if (lexer->data[lexer->index] == '\"' || lexer->data[lexer->index] == '$'
        || lexer->data[lexer->index] == '\\'
        || lexer->data[lexer->index] == '\n'
        || lexer->data[lexer->index] == '`')
    {
        if (lexer->data[lexer->index] != '\n')
        {
            word[*word_index] = lexer->data[lexer->index];
        }
        else
        {
            *word_index -= 1;
        }
        lexer->index += 1;
    }
    else
    {
        word[*word_index] = '\\';
    }
    *word_index += 1;
}
