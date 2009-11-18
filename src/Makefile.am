SUBDIRS=testdata icons data

lib_LTLIBRARIES=libftk.la

LINUX_NATIVE=ftk_display_fb.c ftk_source_input.c
LINUX_X11=ftk_display_x11.c ftk_source_x11.c

libftk_la_SOURCES=\
	ftk_canvas.c          ftk_source_primary.c      ftk_wnd_manager_default.c   \
	fontdata.c            ftk_label.c               ftk_source_timer.c \
	ftk_bitmap.c          ftk_font.c                ftk_main_loop.c     ftk_util.c \
	ftk_bitmap_factory.c  ftk_image_jpeg_decoder.c  ftk_source_idle.c   ftk_widget.c \
	ftk.c                 ftk_image_png_decoder.c   ftk_window.c        ftk_sources_manager.c\
	ftk_globals.c         ftk_button.c              ftk_image.c         ftk_log.c \
	ftk_dialog.c          ftk_status_item.c         ftk_status_panel.c  ftk_style.c \
	ftk_menu_panel.c      ftk_menu_item.c           ftk_app_window.c    ftk_entry.c \
	ftk_text_buffer.c     ftk_icon_cache.c          ftk_animator_expand.c \
	ftk_progress_bar.c    ftk_radio_group.c         ftk_check_button.c ftk_linux.c

noinst_PROGRAMS=label_test widget_test demo_label demo_button demo_image demo_image_button demo_dialog demo_multi_win \
	canvas_test ftk_test bitmap_test font_test primary_source_test\
	bitmap_factory_test main_loop_test png_test log_test util_test \
	sources_manager_test demo_menu demo_entry text_buffer_test icon_cache_test \
	demo_expand demo_progress_bar demo_check_button

if FTK_BACKEND_X11
libftk_la_SOURCES+=$(LINUX_X11)
endif

if FTK_BACKEND_FB
libftk_la_SOURCES+=$(LINUX_NATIVE)
noinst_PROGRAMS+=display_fb_test input_test
display_fb_test_SOURCES=display_fb_test.c
display_fb_test_LDFLAGS=$(MYLDFLAGS)
input_test_SOURCES=input_test.c
input_test_LDFLAGS=$(MYLDFLAGS)
endif

INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"$(datadir)/ftk\"" -DLINUX
MYLDFLAGS=.libs/libftk.a -lpng -ljpeg

demo_check_button_SOURCES=demo_check_button.c
demo_check_button_LDFLAGS=$(MYLDFLAGS)

demo_progress_bar_SOURCES=demo_progress_bar.c
demo_progress_bar_LDFLAGS=$(MYLDFLAGS)

demo_expand_SOURCES=demo_expand.c
demo_expand_LDFLAGS=$(MYLDFLAGS)

icon_cache_test_SOURCES=icon_cache_test.c
icon_cache_test_LDFLAGS=$(MYLDFLAGS)

sources_manager_test_SOURCES=sources_manager_test.c
sources_manager_test_LDFLAGS=$(MYLDFLAGS)

text_buffer_test_SOURCES=text_buffer_test.c
text_buffer_test_LDFLAGS=$(MYLDFLAGS)

util_test_SOURCES=util_test.c
util_test_LDFLAGS=$(MYLDFLAGS)

log_test_SOURCES=log_test.c
log_test_LDFLAGS=$(MYLDFLAGS)

png_test_SOURCES=png_test.c
png_test_LDFLAGS=$(MYLDFLAGS)

font_test_SOURCES=font_test.c
font_test_LDFLAGS=$(MYLDFLAGS)

bitmap_test_SOURCES=bitmap_test.c
bitmap_test_LDFLAGS=$(MYLDFLAGS)

label_test_SOURCES=label_test.c
label_test_LDFLAGS=$(MYLDFLAGS)

widget_test_SOURCE=widget_test.c
widget_test_LDFLAGS=$(MYLDFLAGS)

demo_label_SOURCES=demo_label.c
demo_label_LDFLAGS=$(MYLDFLAGS)

demo_button_SOURCES=demo_button.c
demo_button_LDFLAGS=$(MYLDFLAGS)

demo_image_SOURCES=demo_image.c
demo_image_LDFLAGS=$(MYLDFLAGS)

demo_image_button_SOURCES=demo_image_button.c
demo_image_button_LDFLAGS=$(MYLDFLAGS)

demo_dialog_SOURCES=demo_dialog.c
demo_dialog_LDFLAGS=$(MYLDFLAGS)

demo_multi_win_SOURCES=demo_multi_win.c
demo_multi_win_LDFLAGS=$(MYLDFLAGS)

demo_menu_SOURCES=demo_menu.c
demo_menu_LDFLAGS=$(MYLDFLAGS)

demo_entry_SOURCES=demo_entry.c
demo_entry_LDFLAGS=$(MYLDFLAGS)

canvas_test_SOURCES=canvas_test.c
canvas_test_LDFLAGS=$(MYLDFLAGS)

ftk_test_SOURCES=ftk_test.c
ftk_test_LDFLAGS=$(MYLDFLAGS)

bitmap_factory_test_SOURCES=bitmap_factory_test.c
bitmap_factory_test_LDFLAGS=$(MYLDFLAGS)

main_loop_test_SOURCES=main_loop_test.c
main_loop_test_LDFLAGS=$(MYLDFLAGS)

primary_source_test_SOURCES=primary_source_test.c
primary_source_test_LDFLAGS=$(MYLDFLAGS)

ftkdir=$(includedir)/ftk
ftk_HEADERS=\
	fontdata.h                ftk_log.h\
	ftk_app_window.h          ftk_main_loop.h\
	ftk_bitmap_factory.h      ftk_menu_item.h\
	ftk_bitmap.h              ftk_menu_panel.h\
	ftk_button.h              ftk_platform.h\
	ftk_canvas.h              ftk_source.h\
	ftk_dialog.h              ftk_source_idle.h\
	ftk_display_fb.h          ftk_source_input.h\
	ftk_display.h             ftk_source_primary.h\
	ftk_display_x11.h         ftk_sources_manager.h\
	ftk_event.h               ftk_source_timer.h\
	ftk_font.h                ftk_source_x11.h\
	ftk_gc.h                  ftk_status_item.h\
	ftk_globals.h             ftk_status_panel.h\
	ftk.h                     ftk_style.h\
	ftk_image_decoder.h       ftk_typedef.h\
	ftk_image.h               ftk_util.h\
	ftk_image_jpeg_decoder.h  ftk_widget.h\
	ftk_image_png_decoder.h   ftk_window.h\
	ftk_key.h                 ftk_wnd_manager_default.h\
	ftk_label.h               ftk_wnd_manager.h

EXTRA_DIST=ftk_source_x11.h ftk_source_input.h ftk_display_fb.h \
	ftk_display_x11.h ftk_source_primary.h fontdata.h \
	ftk_image_jpeg_decoder.h ftk_image_png_decoder.h
