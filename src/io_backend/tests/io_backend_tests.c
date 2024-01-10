#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "../io_backend.h"

TestSuite(io_backend, .timeout = 1);

Test(io_backend, io_backend_file)
{
    char *argv[] = {"./42sh", "test_file.txt"};
    char *input = io_backend(2, argv);

    cr_assert_str_eq(input, "echo test", "wrong input: %s", input);
    free(input);
}
