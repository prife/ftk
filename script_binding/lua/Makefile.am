SUBDIRS=lua
bin_PROGRAMS=lua_runner
MYLDFLAGS=$(abs_top_builddir)/src/.libs/libftk.a \
	$(abs_top_builddir)/script_binding/lua/lua/.libs/liblua.a \
	-lpng -ljpeg  -ldl
INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"$(datadir)/ftk\"" \
	-DLINUX -I$(abs_top_srcdir)/src -DTESTDATA_DIR="\"$(abs_top_srcdir)/src/testdata\"" -I$(srcdir)/lua \
	-DLUA_USE_LINUX

lua_runner_SOURCES=ftk_lua.c lua_runner.c
lua_runner_LDFLAGS=$(MYLDFLAGS)
