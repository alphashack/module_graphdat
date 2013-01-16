lib_LTLIBRARIES = libgraphdat-1.0.la

if DARWIN
SYSTEM=osx
endif
if LINUX
SYSTEM=linux
endif

AM_CFLAGS=@MYCFLAGS@
AM_CPPFLAGS=-Ios -Ios/$(SYSTEM) @MYCPPFLAGS@

libgraphdat_1_0_la_SOURCES =	\
	list.c						\
	os/$(SYSTEM)/timehelper.c	\
    os/$(SYSTEM)/mutex.c		\
    os/$(SYSTEM)/thread.c		\
    os/$(SYSTEM)/socket.c		\
    msgpack/src/objectc.c		\
    msgpack/src/unpack.c		\
    msgpack/src/version.c		\
    msgpack/src/vrefbuffer.c	\
    msgpack/src/zone.c			\
	graphdat.c

libgraphdat_1_0_la_LDFLAGS = -module

include_HEADERS = graphdat.h
