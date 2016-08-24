AC_PREREQ([2.69])

AC_INIT([consolation], [0.0.2])

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])

# Check for programs
AC_PROG_CC

PKG_PROG_PKG_CONFIG()
PKG_CHECK_MODULES(LIBINPUT, [libinput >= 1.3.3])
PKG_CHECK_MODULES(LIBUDEV,  [libudev])
PKG_CHECK_MODULES(LIBEVDEV, [libevdev >= 0.4])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
