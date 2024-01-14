/**
 * \file lexer2_tests.c
 * \brief Tests the lexer functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <fnmatch.h>
#include <string.h>

#include "../lexer.h"

TestSuite(lexer2, .timeout = 1);

Test(lexer2, token_and)
{
    struct lexer *lexer = lexer_new("&&");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_AND);
    cr_assert_str_eq(tok.data, "&&");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_or)
{
    struct lexer *lexer = lexer_new("||");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OR);
    cr_assert_str_eq(tok.data, "||");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_pipe)
{
    struct lexer *lexer = lexer_new("|");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_PIPE);
    cr_assert_str_eq(tok.data, "|");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_negate)
{
    struct lexer *lexer = lexer_new("!");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_NEGATE);
    cr_assert_str_eq(tok.data, "!");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_input_redir)
{
    struct lexer *lexer = lexer_new("<");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_INPUT_REDIR);
    cr_assert_str_eq(tok.data, "<");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_output_redir)
{
    struct lexer *lexer = lexer_new(">");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_OUTPUT_REDIR);
    cr_assert_str_eq(tok.data, ">");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_append)
{
    struct lexer *lexer = lexer_new(">>");
    struct token tok = lexer_pop(lexer);
    printf("%s-\n", tok.data);
    printf("%d\n", fnmatch("[0-9]*>>", ">>", 0));
    cr_assert_eq(tok.type, TOKEN_APPEND, "got %d", tok.type);
    cr_assert_str_eq(tok.data, ">>");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input)
{
    struct lexer *lexer = lexer_new("<&");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_DUP_INPUT);
    cr_assert_str_eq(tok.data, "<&");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input_output)
{
    struct lexer *lexer = lexer_new(">&");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_DUP_INPUT_OUTPUT);
    cr_assert_str_eq(tok.data, ">&");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_noclobber)
{
    struct lexer *lexer = lexer_new(">|");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_NOCLOBBER);
    cr_assert_str_eq(tok.data, ">|");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input_output2)
{
    struct lexer *lexer = lexer_new("<>");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_DUP_INPUT_OUTPUT);
    cr_assert_str_eq(tok.data, "<>");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_while)
{
    struct lexer *lexer = lexer_new("while");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WHILE);
    cr_assert_str_eq(tok.data, "while");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_until)
{
    struct lexer *lexer = lexer_new("until");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_UNTIL);
    cr_assert_str_eq(tok.data, "until");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_for)
{
    struct lexer *lexer = lexer_new("for");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_FOR);
    cr_assert_str_eq(tok.data, "for");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_do)
{
    struct lexer *lexer = lexer_new("do");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_DO);
    cr_assert_str_eq(tok.data, "do");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_done)
{
    struct lexer *lexer = lexer_new("done");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_DONE);
    cr_assert_str_eq(tok.data, "done");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_variable_parameters)
{
    struct lexer *lexer = lexer_new("$@");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_VARIABLE);
    cr_assert_str_eq(tok.data, "$@");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_variable_parameters2)
{
    struct lexer *lexer = lexer_new("$*");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_VARIABLE);
    cr_assert_str_eq(tok.data, "$*");
    token_free(tok);
    lexer_free(lexer);
}