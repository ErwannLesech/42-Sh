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

Test(lexer2, token_and2)
{
    struct lexer *lexer = lexer_new("false && true");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "false");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_AND);
    cr_assert_str_eq(tok.data, "&&");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "true");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_and_stick)
{
    struct lexer *lexer = lexer_new("false&&true");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "false");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_AND);
    cr_assert_str_eq(tok.data, "&&");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "true");
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

Test(lexer2, token_pipe2)
{
    struct lexer *lexer = lexer_new("echo papa|tr a e");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "papa");
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
    cr_assert_str_eq(tok.data, "a");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "e");
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
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, "<");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_output_redir)
{
    struct lexer *lexer = lexer_new(">");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, ">");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_append)
{
    struct lexer *lexer = lexer_new(">>");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR, "got %d", tok.type);
    cr_assert_str_eq(tok.data, ">>");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input)
{
    struct lexer *lexer = lexer_new("<&");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, "<&");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input_output)
{
    struct lexer *lexer = lexer_new(">&");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, ">&");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_noclobber)
{
    struct lexer *lexer = lexer_new(">|");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, ">|");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_dup_input_output2)
{
    struct lexer *lexer = lexer_new("<>");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, "<>");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_redir_stick_left)
{
    struct lexer *lexer = lexer_new("ls -la 2>file");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "ls");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "-la");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, ">");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "file");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_redir_stick_left2)
{
    struct lexer *lexer = lexer_new("ls -la 2<>file");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "ls");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "-la");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, "<>");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "file");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_redir_stick_left3)
{
    struct lexer *lexer = lexer_new("ls -la >| file");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "ls");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "-la");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_REDIR);
    cr_assert_str_eq(tok.data, ">|");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "file");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_while)
{
    struct lexer *lexer = lexer_new("while");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "while");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_until)
{
    struct lexer *lexer = lexer_new("until");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "until");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_for)
{
    struct lexer *lexer = lexer_new("for");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "for");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_do)
{
    struct lexer *lexer = lexer_new("do");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "do");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_done)
{
    struct lexer *lexer = lexer_new("done");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "done");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_variable_parameters)
{
    struct lexer *lexer = lexer_new("$@");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$@");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_variable_parameters2)
{
    struct lexer *lexer = lexer_new("$*");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$*");
    token_free(tok);
    lexer_free(lexer);
}

Test(lexer2, token_double_quote)
{
    struct lexer *lexer = lexer_new("echo \"tata toto\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "tata toto", "got %s", tok.data);
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_double_quote_newline)
{
    struct lexer *lexer = lexer_new("echo \"tata \n toto\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "tata \n toto");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_double_quote_escaped)
{
    struct lexer *lexer = lexer_new("echo -e \"tata \\\n toto\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "-e");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "tata \n toto");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_double_quote_variable)
{
    struct lexer *lexer = lexer_new("echo \"$test\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$test");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_double_quote_variable_escaped)
{
    struct lexer *lexer = lexer_new("echo \"\\$test\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$test");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment)
{
    struct lexer *lexer = lexer_new("toto=2");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_in_echo)
{
    struct lexer *lexer = lexer_new("echo toto=2");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_in_echo2)
{
    struct lexer *lexer = lexer_new("echo toto=2 tata=3");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "tata");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "3");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_name)
{
    struct lexer *lexer = lexer_new("_t11oto=2");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "_t11oto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_name1)
{
    struct lexer *lexer = lexer_new("1_t11oto=2");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_ERROR, "got %d", tok.type);
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_name3)
{
    struct lexer *lexer = lexer_new("if=200");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "if");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "200");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, token_word_assignment_name4)
{
    struct lexer *lexer = lexer_new("if=200;else=400;toto=1003994\ntata=200");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "if");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "200");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "else");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "400");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_SEMICOLON);
    cr_assert_str_eq(tok.data, ";");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);
    
    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "1003994");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_EOL);
    cr_assert_str_eq(tok.data, "\n");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "tata");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "200");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, word_assignement5)
{
    struct lexer *lexer = lexer_new("toto=2=3");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "2=3");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, word_assignement6)
{
    struct lexer *lexer = lexer_new("toto=2lmlamdad=3_-");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2lmlamdad=3_-");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, word_assignement7)
{
    struct lexer *lexer = lexer_new("toto=2lmla\\nmdad=3_-$auhi");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2lmlanmdad=3_-");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$auhi");
    token_free(tok);
    
    lexer_free(lexer);
}

Test(lexer2, word_assignement8)
{
    struct lexer *lexer = lexer_new("toto=2lmla\\nmdad=3_-\\$auhi");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD_ASSIGNMENT);
    cr_assert_str_eq(tok.data, "toto");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "2lmlanmdad=3_-$auhi");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, error_word_assignement)
{
    struct lexer *lexer = lexer_new("toto = kdp");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "toto");

    // TO CHECK IF WE SHLOUD RETURN AN ERROR
    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "=");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD, "got %d", tok.type);
    cr_assert_str_eq(tok.data, "kdp");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, variable_find)
{
    struct lexer *lexer = lexer_new("echo $toto");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$toto");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, variable_find2)
{
    struct lexer *lexer = lexer_new("echo \"$a1:dddfff\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$a1");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, ":dddfff");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, variable_find3)
{
    struct lexer *lexer = lexer_new("echo \"$a1_dddfff\"");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$a1_dddfff");
    token_free(tok);

    lexer_free(lexer);
}

Test(lexer2, variable_find4)
{
    struct lexer *lexer = lexer_new("echo \"$a1_dddfff\"$toto");
    struct token tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "echo");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$a1_dddfff");
    token_free(tok);

    tok = lexer_pop(lexer);
    cr_assert_eq(tok.type, TOKEN_WORD);
    cr_assert_str_eq(tok.data, "$toto");
    token_free(tok);

    lexer_free(lexer);
}
