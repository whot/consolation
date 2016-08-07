sbin_PROGRAMS = consolation
consolation_SOURCES = consolation.c consolation.h selection.c action.c input.c shared.c shared.h
consolation_CFLAGS = $(LIBUDEV_CFLAGS) $(LIBEVDEV_CFLAGS) $(LIBINPUT_CFLAGS)
consolation_LDADD  = $(LIBUDEV_LIBS)   $(LIBEVDEV_LIBS)   $(LIBINPUT_LIBS)
