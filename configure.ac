#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([tpeb-library], [1.0], [tbowers@email.arizona.edu])
AM_INIT_AUTOMAKE
LT_PREREQ([2.2])
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([src/atime.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor gettimeofday pow sqrt strchr strstr])

AC_CONFIG_FILES([Makefile
                 include/Makefile
		 src/Makefile])
AC_OUTPUT
