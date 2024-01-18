/**
 * \file io_backend_tests.c
 * \brief Tests the io_backend function.
 * \author Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../io_backend.h"

TestSuite(io_backend, .timeout = 1);

Test(io_backend, io_backend_direct)
{
    char *argv[] = { "./42sh", "-c", "echo test" };
    char *input = io_backend_manager(3, argv);

    cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_direct_multiple)
{
    char *argv[] = { "./42sh", "-c", "echo test ; echo test2" };
    char *input = io_backend_manager(3, argv);

    cr_assert_str_eq(input, "echo test ; echo test2", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_direct_backslash)
{
    char *argv[] = { "./42sh", "-c", "echo test \\" };
    char *input = io_backend_manager(3, argv);

    cr_assert_str_eq(input, "echo test \\", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_file)
{
    char *argv[] = { "./42sh", "../src/io_backend/tests/test.txt" };
    char *input = io_backend_manager(2, argv);

    cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_stdin)
{
    char *argv[] = { "./42sh" };

    pid_t pid = fork();

    if (pid == 0)
    {
        cr_redirect_stdin();
        char *input = io_backend_manager(1, argv);
        cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
        free(input);
        exit(0);
    }
    else
    {
        char *input = "echo test";
        if (write(0, input, strlen(input)) == 0)
            cr_assert_fail("write failed");
        if (write(0, "\n", 1) == 0)
            cr_assert_fail("write failed");
    }
}

Test(io_backend, io_backend_stdin_eof)
{
    char *argv[] = { "./42sh" };

    pid_t pid = fork();

    if (pid == 0)
    {
        cr_redirect_stdin();
        char *input = io_backend_manager(1, argv);
        cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
        free(input);
        exit(0);
    }
    else
    {
        char *input = "echo test";
        if (write(0, input, strlen(input)) == 0)
            cr_assert_fail("write failed");
    }
}
