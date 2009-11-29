include $(abs_top_srcdir)/src/rules.mk

SUBDIRS=lua
bin_PROGRAMS=ftk_run

ftk_run_SOURCES=ftk_lua.c lua_runner.c
ftk_run_LDFLAGS=$(MYLDFLAGS) $(abs_top_builddir)/script_binding/lua/lua/.libs/liblua.a \
	-lpng -ljpeg  -ldl
ftk_run_CFLAGS=-DLUA_USE_LINUX -I$(srcdir)/lua

EXTRA_DIST=ftk_lua.h
