# Init the 42sh project
AC_INIT([42sh], [1.0], [erwann.lesech@epita.fr])

# Setup automake
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

# Pretty display of makefile rules
AM_SILENT_RULES([yes])

# Check if a compiler is available for c
AC_PROG_CC

# Check if a compiler have this list of  flags
# AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -Werror -Wvla -pedantic -std=c99])

MY_CFLAGS="-Wall -Wextra -Werror -Wvla -pedantic -std=c99"
AC_SUBST([MY_CFLAGS])

# Enable ar for Makefile
AM_PROG_AR

# Check if a ranlib is available
AC_PROG_RANLIB

# List Makefile in subdirectories
AC_CONFIG_FILES([
                    Makefile
                    src/lexer/Makefile
                ])
                
AC_OUTPUT