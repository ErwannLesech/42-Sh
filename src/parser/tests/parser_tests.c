/**
 * \file parser_tests.c
 * \brief Tests the parser functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../../ast/ast.h"
#include "../parser.h"

TestSuite(parser, .timeout = 1);

Test(parser, empty_string)
{
    struct lexer *lexer = lexer_new("");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node->type, AST_EMPTY);
    ast_free(node);
    lexer_free(lexer);
}

/*
Test(parser, command_with_if_else)
{
    struct lexer *lexer =
        lexer_new("ls -l; if echo hello; then echo world; else echo bye; fi");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[0]->type, AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[0]->children[0]->type, AST_WORD);
    cr_assert_str_eq(node->children[0]->children[0]->value, "ls");
    cr_assert_eq(node->children[0]->children[1]->type, AST_WORD);
    cr_assert_str_eq(node->children[0]->children[1]->value, "-l");
    cr_assert_eq(node->children[1]->type, AST_CONDITION);
    cr_assert_eq(node->children[1]->children[0]->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[0]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]->children[0]->children[0]->children[0]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[0]->children[0]->children[0]->value,
        "echo");
    cr_assert_eq(node->children[1]->children[0]->children[0]->children[1]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[0]->children[0]->children[1]->value,
        "hello");
    cr_assert_eq(node->children[1]->children[1]->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[1]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]->children[1]->children[0]->children[0]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[1]->children[0]->children[0]->value,
        "echo");
    cr_assert_eq(node->children[1]->children[1]->children[0]->children[1]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[1]->children[0]->children[1]->value,
        "world");
    cr_assert_eq(node->children[1]->children[2]->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[2]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]->children[2]->children[0]->children[0]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[2]->children[0]->children[0]->value,
        "echo");
    cr_assert_eq(node->children[1]->children[2]->children[0]->children[1]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[2]->children[0]->children[1]->value, "bye");
    lexer_free(lexer);
    ast_free(node);
}
*/
/*
Test(parser, command_with_if_elif)
{
    struct lexer *lexer = lexer_new("ls -l; if echo hello; then echo world; "
                                    "elif echo bye; then echo world; fi");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[0]->type, AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[0]->children[0]->type, AST_WORD);
    cr_assert_str_eq(node->children[0]->children[0]->value, "ls");
    cr_assert_eq(node->children[0]->children[1]->type, AST_WORD);
    cr_assert_str_eq(node->children[0]->children[1]->value, "-l");
    cr_assert_eq(node->children[1]->type, AST_CONDITION);
    cr_assert_eq(node->children[1]->children[0]->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[0]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]->children[0]->children[0]->children[0]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[0]->children[0]->children[0]->value,
        "echo");
    cr_assert_eq(node->children[1]->children[0]->children[0]->children[1]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[0]->children[0]->children[1]->value,
        "hello");
    cr_assert_eq(node->children[1]->children[1]->type, AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[1]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]->children[1]->children[0]->children[0]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[1]->children[0]->children[0]->value,
        "echo");
    cr_assert_eq(node->children[1]->children[1]->children[0]->children[1]->type,
                 AST_WORD);
    cr_assert_str_eq(
        node->children[1]->children[1]->children[0]->children[1]->value,
        "world");
    cr_assert_eq(node->children[1]->children[2]->type, AST_CONDITION);
    cr_assert_eq(node->children[1]->children[2]->children[0]->type,
                 AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[2]->children[0]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]
                     ->children[2]
                     ->children[0]
                     ->children[0]
                     ->children[0]
                     ->type,
                 AST_WORD);
    cr_assert_str_eq(node->children[1]
                         ->children[2]
                         ->children[0]
                         ->children[0]
                         ->children[0]
                         ->value,
                     "echo");
    cr_assert_eq(node->children[1]
                     ->children[2]
                     ->children[0]
                     ->children[0]
                     ->children[1]
                     ->type,
                 AST_WORD);
    cr_assert_str_eq(node->children[1]
                         ->children[2]
                         ->children[0]
                         ->children[0]
                         ->children[1]
                         ->value,
                     "bye");
    cr_assert_eq(node->children[1]->children[2]->children[1]->type,
                 AST_COMMAND_LIST);
    cr_assert_eq(node->children[1]->children[2]->children[1]->children[0]->type,
                 AST_SIMPLE_COMMAND);
    cr_assert_eq(node->children[1]
                     ->children[2]
                     ->children[1]
                     ->children[0]
                     ->children[0]
                     ->type,
                 AST_WORD);
    cr_assert_str_eq(node->children[1]
                         ->children[2]
                         ->children[1]
                         ->children[0]
                         ->children[0]
                         ->value,
                     "echo");
    cr_assert_eq(node->children[1]
                     ->children[2]
                     ->children[1]
                     ->children[0]
                     ->children[1]
                     ->type,
                 AST_WORD);
    cr_assert_str_eq(node->children[1]
                         ->children[2]
                         ->children[1]
                         ->children[0]
                         ->children[1]
                         ->value,
                     "world");
    lexer_free(lexer);
    ast_free(node);
}
*/
Test(parser, error)
{
    struct lexer *lexer = lexer_new("ls -l; if echo hello; then echo world; "
                                    "elif echo bye; then echo world;");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node, NULL);
    lexer_free(lexer);
    ast_free(node);
}

Test(parser, error1)
{
    struct lexer *lexer = lexer_new("ls -l; echo hello; then echo world; elif "
                                    "echo bye; then echo world; fi;");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node, NULL);
    lexer_free(lexer);
    ast_free(node);
}

Test(parser, error2)
{
    struct lexer *lexer = lexer_new("ls -l; if echo hello; echo world; elif "
                                    "echo bye; then echo world; fi;");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node, NULL);
    lexer_free(lexer);
    ast_free(node);
}

Test(parser, error3)
{
    struct lexer *lexer = lexer_new("ls -l; if echo hello; then echo world; "
                                    "echo bye; then echo world; fi;");
    struct ast_node *node = parse(lexer);
    cr_assert_eq(node, NULL);
    lexer_free(lexer);
    ast_free(node);
}

Test(parser, error4)
{
    struct lexer *lexer = lexer_new("ls -l; if echo hello; then echo world; "
                                    "elif echo bye; echo world; fi;");
    struct ast_node *node = parse(lexer);
    print_ast(node, 0, false);
    cr_assert_eq(node, NULL);
    lexer_free(lexer);
    ast_free(node);
}
