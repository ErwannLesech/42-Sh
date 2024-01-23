/**
 * \file lexer_tests.c
 * \brief Tests the lexer functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "lexer/lexer.h"

TestSuite(lexer3, .timeout=20);

Test(lexer3, command_block_simple)
{
    struct lexer *lexer = lexer_new("{ echo a; echo b; } | tr b h");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_PIPE);
    cr_assert_str_eq(tok.data, "|");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "tr");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "h");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, command_block_stick_simple)
{
    struct lexer *lexer = lexer_new("{ echo a; echo b; }|tr b h");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_PIPE);
    cr_assert_str_eq(tok.data, "|");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "tr");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "h");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, command_block_nested)
{
    struct lexer *lexer = lexer_new("{ echo a; { echo b; } } | tr b h");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_PIPE);
    cr_assert_str_eq(tok.data, "|");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "tr");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "b");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "h");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, function_simple)
{
    struct lexer *lexer = lexer_new("foo() { echo test }");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "foo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_PAR);
    cr_assert_str_eq(tok.data, "(");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_PAR);
    cr_assert_str_eq(tok.data, ")");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "test");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);    

    lexer_free(lexer);
}

Test(lexer3, function_simple2)
{
    struct lexer *lexer = lexer_new("foo() {\
                                        bar() {\
                                            echo foobar\
                                        }\
                                    }");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "foo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_PAR);
    cr_assert_str_eq(tok.data, "(");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_PAR);
    cr_assert_str_eq(tok.data, ")");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "bar");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_PAR);
    cr_assert_str_eq(tok.data, "(");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_PAR);
    cr_assert_str_eq(tok.data, ")");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_BRACES);
    cr_assert_str_eq(tok.data, "{");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "foobar");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_BRACES);
    cr_assert_str_eq(tok.data, "}");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, subshell_simple)
{
    struct lexer *lexer = lexer_new("a=sh; (a=42; echo -n $a);echo $a");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "sh");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OPEN_PAR);
    cr_assert_str_eq(tok.data, "(");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "42");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "-n");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_VARIABLE);
    cr_assert_str_eq(tok.data, "$a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_CLOSE_PAR);
    cr_assert_str_eq(tok.data, ")");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_VARIABLE);
    cr_assert_str_eq(tok.data, "$a");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, command_substitution_simple)
{
    struct lexer *lexer = lexer_new("echo $(echo test)");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SUBSTITUTION, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "echo test");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, command_substitution_simple2)
{
    struct lexer *lexer = lexer_new("echo $(echo $(echo test))");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SUBSTITUTION, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "echo $(echo test)");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer3, variable_substitution)
{
    struct lexer *lexer = lexer_new("a=$(cat file)");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SUBSTITUTION, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "cat file");
    token_free(tok);

    lexer_free(lexer);
}

/*Test(lexer3, command_substitution_double_quote)
{
    struct lexer *lexer = lexer_new("echo \"$(echo test)\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SUBSTITUTION, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "echo test");
    token_free(tok);

    lexer_free(lexer);
}*/