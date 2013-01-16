AC_INIT([libgraphdat], [1.0], [support@graphdat.com], [libgraphdat],[http://www.graphdat.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([config.h])
AC_ENABLE_SHARED(yes)
AC_ENABLE_STATIC(no)
LT_INIT

#
# OS detect
#
if uname | grep Darwin >config.tmp
then
	darwin=true
	MYCPPFLAGS="-I./msgpack/src"
	AC_DEFINE([OS_DARWIN],[1],[on osx])
	AC_DEFINE([DARWIN],[1],[on osx])
fi
AM_CONDITIONAL(DARWIN, test x$darwin = xtrue)

if uname | grep Linux >config.tmp
then
	linux=true
	AC_DEFINE([OS_LINUX],[1],[on linux])
	AC_DEFINE([LINUX],[1],[on linux])
fi
AM_CONDITIONAL(LINUX, test x$linux = xtrue)

AC_CONFIG_FILES([Makefile])

AC_SUBST(MYCPPFLAGS)
AC_SUBST(MYCFLAGS)

AC_OUTPUT
