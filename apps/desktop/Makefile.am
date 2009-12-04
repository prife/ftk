SUBDIRS=icons xul
include $(abs_top_srcdir)/src/rules.mk

noinst_PROGRAMS=app_info_test
app_info_test_SOURCES=app_info.c
app_info_test_CFLAGS=-DAPP_INFO_TEST
app_info_test_LDFLAGS=$(MYLDFLAGS)

bin_PROGRAMS=desktop
desktop_SOURCES=main.c app_info.c
desktop_LDFLAGS=$(MYLDFLAGS)
desktop_CFLAGS=-DAPP_DATA_DIR="\"$(datadir)/ftk/desktop\"" \
	-DAPP_LOCAL_DATA_DIR="\"$(srcdir)\""

EXTRA_DIST=app_info.h
