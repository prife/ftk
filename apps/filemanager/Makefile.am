SUBDIRS=icons data
include $(abs_top_srcdir)/src/rules.mk

LOCAL_CFLAGS=-DAPP_DATA_DIR="\"$(datadir)/ftk/filem\"" -DAPP_LOCAL_DATA_DIR="\"$(srcdir)\""
lib_LTLIBRARIES=libapp-filem.la
libapp_filem_la_SOURCES=main.c
libapp_filem_la_LDFLAGS=$(MYLDFLAGS)
libapp_filem_la_CFLAGS=$(LOCAL_CFLAGS)

noinst_PROGRAMS=filem
filem_SOURCES=main.c
filem_LDFLAGS=$(MYLDFLAGS)
filem_CFLAGS=$(LOCAL_CFLAGS) -DHAS_MAIN

