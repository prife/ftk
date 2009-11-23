SUBDIRS=. testdata icons data demos test

lib_LTLIBRARIES=libftk.la

LINUX_NATIVE=ftk_display_fb.c ftk_source_input.c
LINUX_X11=ftk_display_x11.c ftk_source_x11.c

libftk_la_SOURCES=\
	ftk_canvas.c          ftk_source_primary.c      ftk_wnd_manager_default.c   \
	fontdata.c            ftk_label.c               ftk_source_timer.c \
	ftk_bitmap.c          ftk_font_default.c        ftk_main_loop.c     ftk_util.c \
	ftk_bitmap_factory.c  ftk_image_jpeg_decoder.c  ftk_source_idle.c   ftk_widget.c \
	ftk.c                 ftk_image_png_decoder.c   ftk_window.c        ftk_sources_manager.c\
	ftk_globals.c         ftk_button.c              ftk_image.c         ftk_log.c \
	ftk_dialog.c          ftk_status_item.c         ftk_status_panel.c  ftk_style.c \
	ftk_menu_panel.c      ftk_menu_item.c           ftk_app_window.c    ftk_entry.c \
	ftk_text_buffer.c     ftk_icon_cache.c          ftk_animator_expand.c \
	ftk_progress_bar.c    ftk_radio_group.c         ftk_check_button.c  ftk_linux.c \
	ftk_gesture.c         ftk_wait_box.c            ftk_mmap_linux.c    ftk_scroll_bar.c \
	ftk_sprite.c          ftk_display.c             ftk_xml_parser.c    ftk_xul.c

if FTK_BACKEND_FB
libftk_la_SOURCES+=$(LINUX_NATIVE)
endif

if FTK_BACKEND_X11
libftk_la_SOURCES+=$(LINUX_X11)
endif

INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"$(datadir)/ftk\"" \
	-DLOCAL_DATA_DIR="\"$(abs_top_srcdir)/src\"" -DLINUX -DFTK_SUPPORT_C99

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
