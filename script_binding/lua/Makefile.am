include $(abs_top_srcdir)/src/rules.mk

SUBDIRS=lua
bin_PROGRAMS=ftk_run

ftk_run_SOURCES=ftk_lua.c lua_runner.c tolua_event.c  tolua_is.c  tolua_map.c  tolua_push.c  tolua_to.c \
	lua_ftk.c lua_ftk_typedef.c lua_ftk_widget.c lua_ftk_window.c lua_ftk_xul.c lua_ftk_callbacks.c \
	lua_ftk_app_window.c lua_ftk_label.c lua_ftk_button.c lua_ftk_entry.c \
	lua_ftk_source_timer.c lua_ftk_main_loop.c lua_ftk_source_idle.c lua_ftk_source.c  \
	lua_ftk_combo_box.c

ftk_run_LDFLAGS=$(MYLDFLAGS) $(abs_top_builddir)/script_binding/lua/lua/.libs/liblua.a \
	-lpng -ljpeg  -ldl
ftk_run_CFLAGS=-DLUA_USE_LINUX -I$(srcdir)/lua

EXTRA_DIST=ftk_lua.h t1.lua
