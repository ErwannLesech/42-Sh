#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../options.h"

TestSuite(options, .timeout = 1);

Test(options, last_arg_logger)
{
    char *argv[] = {"./42sh", "echo test", "--logger"};
    int argc = 3;

    bool logger_enabled = check_logger(&argc, argv);

    cr_assert_eq(logger_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, middle_arg_logger)
{
    char *argv[] = {"./42sh", "--logger", "echo test"};
    int argc = 3;

    bool logger_enabled = check_logger(&argc, argv);

    cr_assert_eq(logger_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, last_arg_pretty_print)
{
    char *argv[] = {"./42sh", "echo test", "--pretty-print"};
    int argc = 3;

    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, middle_arg_pretty_print)
{
    char *argv[] = {"./42sh", "--pretty-print", "echo test"};
    int argc = 3;

    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

/* Combine options */

Test(options, both_last_args)
{
    char *argv[] = {"./42sh", "echo test", "--logger", "--pretty-print"};
    int argc = 4;

    bool logger_enabled = check_logger(&argc, argv);
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(logger_enabled, true);
    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, both_middle_args)
{
    char *argv[] = {"./42sh", "--logger", "--pretty-print", "echo test"};
    int argc = 4;

    bool logger_enabled = check_logger(&argc, argv);
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(logger_enabled, true);
    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

Test(options, separated_args)
{
    char *argv[] = {"./42sh", "--logger", "echo test", "--pretty-print"};
    int argc = 4;

    bool logger_enabled = check_logger(&argc, argv);
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(logger_enabled, true);
    cr_assert_eq(pretty_print_enabled, true);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}

/* No options */

Test(options, no_options)
{
    char *argv[] = {"./42sh", "echo test"};
    int argc = 2;

    bool logger_enabled = check_logger(&argc, argv);
    bool pretty_print_enabled = check_pretty_print(&argc, argv);

    cr_assert_eq(logger_enabled, false);
    cr_assert_eq(pretty_print_enabled, false);
    cr_assert_eq(argc, 2, "wrong argc: %d", argc);
    cr_assert_str_eq(argv[1], "echo test", "wrong argv[1]: %s", argv[1]);
}
