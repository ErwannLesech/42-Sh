#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../io_backend.h"

TestSuite(io_backend, .timeout = 1);

Test(io_backend, io_backend_direct)
{
    char *argv[] = {"./42sh", "echo test"};
    char *input = io_backend(2, argv);

    cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_direct_multiple)
{
    char *argv[] = {"./42sh", "echo test ; echo test2"};
    char *input = io_backend(2, argv);

    cr_assert_str_eq(input, "echo test ; echo test2", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_direct_backslash)
{
    char *argv[] = {"./42sh", "echo test \\"};
    char *input = io_backend(2, argv);

    cr_assert_str_eq(input, "echo test \\", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_file)
{
    char *argv[] = {"./42sh", "-c", "tests/test.txt"};
    char *input = io_backend(3, argv);

    cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
    free(input);
}

Test(io_backend, io_backend_stdin)
{
    char *argv[] = {"./42sh"};
    
    pid_t pid = fork();

    if (pid == 0)
    {
        cr_redirect_stdin();
        char *input = io_backend(1, argv);
        cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
        free(input);
        exit(0);
    }
    else
    {
        char *input = "echo test";
        write(0, input, strlen(input));
        write(0, "\n", 1);
    }    
}

Test(io_backend, io_backend_stdin_eof)
{
    char *argv[] = {"./42sh"};
    
    pid_t pid = fork();

    if (pid == 0)
    {
        cr_redirect_stdin();
        char *input = io_backend(1, argv);
        cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
        free(input);
        exit(0);
    }
    else
    {
        char *input = "echo test";
        write(0, input, strlen(input));
    }    
}
