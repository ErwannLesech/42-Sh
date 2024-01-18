/**
 * \file options_tests.c
 * \brief Tests the options functions.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "options/options.h"

TestSuite(options, .timeout = 10);

Test(options, last_arg_pretty_print)
{
    char *argv[] = { "./42sh", "echo test", "--pretty-print" };
    int argc = 3;

    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, middle_arg_pretty_print)
{
    char *argv[] = { "./42sh", "--pretty-print", "echo test" };
    int argc = 3;

    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

/* No options */

Test(options, no_options)
{
    char *argv[] = { "./42sh", "echo test" };
    int argc = 2;

    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(pretty_print_enabled, false);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}
