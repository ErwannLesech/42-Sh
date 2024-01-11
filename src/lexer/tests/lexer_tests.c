#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../lexer.h"

TestSuite(lexer, .timeout = 1);

Test(lexer, lexer_new_and_free)
{
    struct lexer *lexer = lexer_new("test");
    cr_assert_not_null(lexer);
    cr_assert_eq(lexer->index, 0);
    cr_assert_str_eq(lexer->data, "test");
    lexer_free(lexer);
}

Test(lexer, get_word_simple)
{
    struct lexer *lexer = lexer_new("Ceci est un mot.");
    char *word = get_word(lexer, NULL);
    cr_assert_str_eq(word, "Ceci");
    cr_assert_eq(lexer->index, 5, "index = %lu", lexer->index);
    free(word);
    lexer_free(lexer);
}

Test(lexer, lexer_peek_simple)
{
    struct lexer *lexer = lexer_new("Ceci est un mot.");
    struct token token = lexer_peek(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "Ceci");
    cr_assert_eq(lexer->index, 0);
    token_free(token);
    lexer_free(lexer);
}

Test(lexer, lexer_pop_simple)
{
    struct lexer *lexer = lexer_new("Ceci est un mot.");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "Ceci");
    cr_assert_eq(lexer->index, 5);
    token_free(token);
    
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "est");
    cr_assert_eq(lexer->index, 9);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "un");
    cr_assert_eq(lexer->index, 12);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "mot.");
    cr_assert_eq(lexer->index, 16);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 17, "index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(lexer, lexer_pop_with_semicolon)
{
    struct lexer *lexer = lexer_new("echo ; ls");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.data, "echo");
    cr_assert_eq(lexer->index, 5);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(token.data, ";");
    cr_assert_eq(lexer->index, 7);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "ls", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 9, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 10);
    token_free(token);

    lexer_free(lexer);
}

Test(lexer, lexer_pop_with_backslash_semicolon)
{
    struct lexer *lexer = lexer_new("echo \\; ls");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, ";", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 8, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "ls", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 10, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 11);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_pop_with_backslash_semicolon_in_word)
{
    struct lexer *lexer = lexer_new("echo \\;ls");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, ";ls", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 9, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 10);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_pop_with_single_quote)
{
    struct lexer *lexer = lexer_new("echo ';'");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, ";", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 8, "lexer->index = %lu", lexer->index);
    token_free(token);
    
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 9);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_pop_with_backslash_single_quote)
{
    struct lexer *lexer = lexer_new("echo \\'");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "'", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 7, "lexer->index = %lu", lexer->index);
    token_free(token);
    
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 8);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_pop_with_backslash_single_quote_2)
{
    struct lexer *lexer = lexer_new("echo test\\'lsets");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "test'lsets", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 16, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 17);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_pop_with_backslash_diactivate_single_quote)
{
    struct lexer *lexer = lexer_new("echo test\\'ls\\'ets");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "test'ls'ets", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 18, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 19);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, complex_single_quotes_backslash)
{
    struct lexer *lexer = lexer_new("echo \\'\\'\\dkd\\'");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "''dkd'", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 15, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, multiple_spaces)
{
    struct lexer *lexer = lexer_new("echo   test");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 7, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);
    
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "test", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 11, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_if_then)
{
    struct lexer *lexer = lexer_new("if 1=1 then echo 'ok' fi;");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_IF, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "if", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 3, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "1=1", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 7, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_THEN, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "then", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 12, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 17, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "ok", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 22, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_FI, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "fi", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 24, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_SEMICOLON, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, ";", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 25, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_elif)
{
    struct lexer *lexer = lexer_new("elif 1=1");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_ELIF, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "elif", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_else)
{
    struct lexer *lexer = lexer_new("else");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_ELSE, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "else", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 4, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, lexer_backslash_n)
{
    struct lexer *lexer = lexer_new("echo test\nif");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "test", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 9, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOL, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "\n", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 10, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_IF, "token.type = %d", token.type, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "if", "token.data = %s", token.data, "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 12, "lexer->index = %lu", lexer->index, "lexer->index = %lu", lexer->index);
    token_free(token);
    
    lexer_free(lexer);
}

Test(Lexer, simple_comment)
{
    struct lexer *lexer = lexer_new("echo # Ceci est un commentaire");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 31);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, comment_with_semicolon)
{
    struct lexer *lexer = lexer_new("echo # Ceci est un commentaire; echo");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 37, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, comment_with_backslash_n)
{
    struct lexer *lexer = lexer_new("echo # Ceci est un commentaire\n echo");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOL, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "\n", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 32, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 36, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 37, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

Test(Lexer, comment_with_back_slash2)
{
    struct lexer *lexer = lexer_new("echo # Ceci est un commentaire\\n echo");
    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "echo", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 5, "lexer->index = %lu", lexer->index);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "\0");
    cr_assert_eq(lexer->index, 38, "lexer->index = %lu", lexer->index);
    token_free(token);

    lexer_free(lexer);
}

/* ERROR CASES */

Test(Lexer, only_one_single_quote)
{
    struct lexer *lexer = lexer_new("echo '");
    struct token token = lexer_pop(lexer);
    token_free(token);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_ERROR, "token.type = %d", token.type);
    cr_assert_str_eq(token.data, "get_word - Missing closing quote.", "token.data = %s", token.data);
    cr_assert_eq(lexer->index, 6, "lexer->index = %lu", lexer->index);

    lexer_free(lexer);
}
