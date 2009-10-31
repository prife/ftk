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
	ftk_menu_panel.c      ftk_menu_item.c           ftk_app_window.c

noinst_PROGRAMS=label_test widget_test demo1 demo2 demo3 demo4 demo5 demo6 \
	canvas_test ftk_test bitmap_test font_test primary_source_test\
	bitmap_factory_test main_loop_test png_test log_test util_test \
	sources_manager_test demo7

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

INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\""
MYLDFLAGS=.libs/libftk.a

sources_manager_test_SOURCES=sources_manager_test.c
sources_manager_test_LDFLAGS=$(MYLDFLAGS)

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

demo1_SOURCES=demo1.c
demo1_LDFLAGS=$(MYLDFLAGS)

demo2_SOURCES=demo2.c
demo2_LDFLAGS=$(MYLDFLAGS)

demo3_SOURCES=demo3.c
demo3_LDFLAGS=$(MYLDFLAGS)

demo4_SOURCES=demo4.c
demo4_LDFLAGS=$(MYLDFLAGS)

demo5_SOURCES=demo5.c
demo5_LDFLAGS=$(MYLDFLAGS)

demo6_SOURCES=demo6.c
demo6_LDFLAGS=$(MYLDFLAGS)

demo7_SOURCES=demo7.c
demo7_LDFLAGS=$(MYLDFLAGS)

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
	ftk_bitmap_factory.h  ftk.h                     ftk_source_idle.h\
	ftk_bitmap.h          ftk_image_decoder.h       \
	ftk_button.h          ftk_image.h               \
	ftk_canvas.h          ftk_source_timer.h\
	ftk_display.h         ftk_key.h                 ftk_style.h\
	ftk_label.h           ftk_typedef.h\
	ftk_event.h           ftk_log.h                 ftk_util.h\
	ftk_font.h            ftk_main_loop.h           ftk_widget.h\
	ftk_gc.h              ftk_platform.h            ftk_window.h\
	ftk_globals.h         ftk_source.h              ftk_wnd_manager.h

EXTRA_DIST=ftk_source_x11.h ftk_source_input.h ftk_display_fb.h \
	ftk_display_x11.h ftk_source_primary.h fontdata.h \
	ftk_image_jpeg_decoder.h ftk_image_png_decoder.h
