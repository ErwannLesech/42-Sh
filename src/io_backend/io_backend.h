#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * \brief Manage the input.
*/
char *io_backend(int argc, char **argv);

#endif /* ! IO_BACKEND_H  */
