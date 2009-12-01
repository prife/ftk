include $(abs_top_srcdir)/src/rules.mk

bin_PROGRAMS=desktop
desktop_SOURCES=main.c
desktop_LDFLAGS=$(MYLDFLAGS)
desktop_CFLAGS=-DAPP_DATA_DIR="\"$(datadir)/ftk/desktop\"" \
	-DAPP_LOCAL_DATA_DIR="\"$(srcdir)\""

