include $(abs_top_srcdir)/src/rules.mk

SUBDIRS=. os backend compiler testdata icons data demos test

lib_LTLIBRARIES=libftk.la

LINUX_NATIVE=$(srcdir)/backend/fb/ftk_display_fb.c \
	$(srcdir)/backend/fb/ftk_source_input.c \
	$(srcdir)/backend/fb/ftk_backend_fb.c

LINUX_X11=$(srcdir)/backend/x11-emu/ftk_display_x11.c \
	$(srcdir)/backend/x11-emu/ftk_source_x11.c \
	$(srcdir)/backend/x11-emu/ftk_backend_x11.c

LINUX_DFB=$(srcdir)/backend/directfb/ftk_display_dfb.c \
	$(srcdir)/backend/directfb/ftk_source_dfb.c \
	$(srcdir)/backend/directfb/ftk_backend_dfb.c

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
	ftk_progress_bar.c    ftk_radio_group.c         ftk_check_button.c  \
	$(srcdir)/os/linux/ftk_mmap_linux.c  \
	$(srcdir)/os/linux/ftk_linux.c  \
	ftk_gesture.c         ftk_wait_box.c  ftk_scroll_bar.c \
	ftk_sprite.c          ftk_display.c             ftk_xml_parser.c    ftk_xul.c \
	ftk_expr.c            ftk_list_view.c           ftk_list_model_default.c \
	ftk_list_render_default.c

if FTK_BACKEND_FB
libftk_la_SOURCES+=$(LINUX_NATIVE)
endif

if FTK_BACKEND_X11
libftk_la_SOURCES+=$(LINUX_X11)
endif

if FTK_BACKEND_DFB
libftk_la_SOURCES+=$(LINUX_DFB)
endif

ftkdir=$(includedir)/ftk
ftk_HEADERS=\
	fontdata.h                ftk_list_render_default.h\
	ftk_animator_expand.h     ftk_list_render.h\
	ftk_animator.h            ftk_list_view.h\
	ftk_app_window.h          ftk_log.h\
	ftk_backend.h             ftk_main_loop.h\
	ftk_bitmap_factory.h      ftk_menu_item.h\
	ftk_bitmap.h              ftk_menu_panel.h\
	ftk_button.h              ftk_mmap.h\
	ftk_canvas.h              ftk_platform.h\
	ftk_check_button.h        ftk_progress_bar.h\
	ftk_clipboard.h           ftk_radio_group.h\
	ftk_dialog.h              ftk_scroll_bar.h\
	ftk_display.h             ftk_source.h\
	ftk_entry.h               ftk_source_idle.h\
	ftk_event.h               ftk_source_primary.h\
	ftk_expr.h                ftk_sources_manager.h\
	ftk_font_default.h        ftk_source_timer.h\
	ftk_font.h                ftk_sprite.h\
	ftk_gc.h                  ftk_status_item.h\
	ftk_gesture.h             ftk_status_panel.h\
	ftk_gesture_listener.h    ftk_style.h\
	ftk_globals.h             ftk_text_buffer.h\
	ftk.h                     ftk_typedef.h\
	ftk_icon_cache.h          ftk_util.h\
	ftk_image_bmp_decoder.h   ftk_wait_box.h\
	ftk_image_decoder.h       ftk_widget.h\
	ftk_image.h               ftk_win32.h\
	ftk_image_jpeg_decoder.h  ftk_window.h\
	ftk_image_png_decoder.h   ftk_wnd_manager_default.h\
	ftk_key.h                 ftk_wnd_manager.h\
	ftk_label.h               ftk_xml_builder.h\
	ftk_list_model_default.h  ftk_xml_parser.h\
	ftk_list_model.h          ftk_xul.h

EXTRA_DIST=	rules.mk
