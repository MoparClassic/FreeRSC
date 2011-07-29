AC_INIT([FreeRSC], [0.1], [lothyis@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/config.h])

AC_CHECK_HEADERS(stdint.h sys/inotify.h sys/epoll.h sys/event.h port.h poll.h sys/select.h sys/eventfd.h sys/signalfd.h dlfcn.h strings.h) 
 
AC_CHECK_FUNCS(inotify_init epoll_ctl kqueue port_create poll select eventfd signalfd)
 
AC_CHECK_FUNCS(clock_gettime, [], [ 
   dnl on linux, try syscall wrapper first
   if test $(uname) = Linux; then
      AC_MSG_CHECKING(for clock_gettime syscall)
      AC_LINK_IFELSE([AC_LANG_PROGRAM(
                      [#include <unistd.h>
                       #include <sys/syscall.h>
                       #include <time.h>],
                      [struct timespec ts; int status = syscall (SYS_clock_gettime, CLOCK_REALTIME, &ts)])],
                     [ac_have_clock_syscall=1
                      AC_DEFINE(HAVE_CLOCK_SYSCALL, 1, "use syscall interface for clock_gettime")
                      AC_MSG_RESULT(yes)],
                     [AC_MSG_RESULT(no)])
   fi
   if test -z "$LIBEV_M4_AVOID_LIBRT" && test -z "$ac_have_clock_syscall"; then
      AC_CHECK_LIB(rt, clock_gettime) 
      unset ac_cv_func_clock_gettime
      AC_CHECK_FUNCS(clock_gettime)
   fi
])

AC_CHECK_FUNCS(nanosleep, [], [ 
   if test -z "$LIBEV_M4_AVOID_LIBRT"; then
      AC_CHECK_LIB(rt, nanosleep) 
      unset ac_cv_func_nanosleep
      AC_CHECK_FUNCS(nanosleep)
   fi
])

AC_CHECK_LIB(m, ceil)

AC_CHECK_FUNCS([strlcat strlcpy strdup])
AC_CONFIG_FILES([Makefile libs/Makefile libs/libev/Makefile src/Makefile tests/Makefile])
AC_OUTPUT
