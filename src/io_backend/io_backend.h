/**
 * \file io_backend.h
 * \brief Header of the io_backend
 * \authors Erwann Lesech, Valentin Gibbe, Ugo Majer, Alexandre Privat
 * \version 1.0
 * \date 12/01/2024
 */

#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * \brief Manage the input.
 * \param argc Number of arguments
 * \param argv Arguments
 * \return the input as a string
 */
char *io_backend(int argc, char **argv);

#endif /* ! IO_BACKEND_H  */
