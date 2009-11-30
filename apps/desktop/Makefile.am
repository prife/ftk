include $(abs_top_srcdir)/src/rules.mk
applibdir=$(libdir)/ftk_apps
applib_LTLIBRARIES=libdesktop.la
libdesktop_la_SOURCES=main.c
libdesktop_la_LDFLAGS=$(MYLDFLAGS)
libdesktop_la_CFLAGS=-DDESKTOP_DATA_DIR="\"$(datadir)/ftk/desktop\"" \
	-DDESKTOP_LOCAL_DATA_DIR="\"$(srcdir)\""

