include $(abs_top_srcdir)/src/rules.mk

SUBDIRS=. im os backend compiler demos test

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

LINUX_SIGMA=$(srcdir)/backend/sigma/ftk_backend_sigma.c \
	$(srcdir)/backend/fb/ftk_source_input.c \
	$(srcdir)/backend/sigma/ftk_display_sigma.c

PSP=$(srcdir)/backend/psp/ftk_backend_psp.c \
	$(srcdir)/backend/psp/ftk_display_psp.c

OS_LINUX=\
	$(srcdir)/os/linux/ftk_mmap_linux.c  \
	$(srcdir)/os/linux/ftk_linux.c 
OS_PSP=\
	$(srcdir)/os/psp/ftk_mmap_psp.c \
	$(srcdir)/os/psp/ftk_psp.c 

libftk_la_SOURCES=\
	ftk_canvas.c          ftk_source_primary.c      ftk_wnd_manager_default.c   \
	ftk_label.c           ftk_source_timer.c \
	ftk_bitmap.c          ftk_main_loop.c     ftk_util.c \
	ftk_bitmap_factory.c  ftk_image_jpeg_decoder.c  ftk_source_idle.c   ftk_widget.c \
	ftk.c                 ftk_image_png_decoder.c   ftk_window.c        ftk_sources_manager.c\
	ftk_globals.c         ftk_button.c              ftk_image.c         ftk_log.c \
	ftk_dialog.c          ftk_status_item.c         ftk_status_panel.c  \
	ftk_menu_panel.c      ftk_menu_item.c           ftk_app_window.c    ftk_entry.c \
	ftk_text_buffer.c     ftk_icon_cache.c          ftk_animator_expand.c ftk_animator_zoom.c \
	ftk_progress_bar.c    ftk_radio_group.c         ftk_check_button.c  \
	ftk_gesture.c         ftk_wait_box.c  ftk_scroll_bar.c \
	ftk_sprite.c          ftk_display.c             ftk_xml_parser.c    ftk_xul.c \
	ftk_expr.c            ftk_list_view.c           ftk_list_model_default.c \
	ftk_list_render_default.c ftk_icon_view.c ftk_theme.c ftk_painter.c \
	ftk_text_view.c       ftk_pipe_socket.c        ftk_message_box.c \
	ftk_popup_menu.c      ftk_animator_alpha.c     ftk_combo_box.c \
	ftk_clipboard.c       ftk_input_method_manager.c \
	ftk_allocator.c            ftk_allocator_default.c \
	ftk_allocator_profile.c  ftk_image_bmp_decoder.c \
	im/ftk_lookup_table.c ftk_input_method_preeditor.c \
	ftk_input_method_preeditor_default.c \
	im/ftk_input_method_py.c im/ftk_input_method_util.c \
	ftk_main_loop_select.c ftk_config.c ftk_display_mem.c

if ENABLE_HANDWRITE
libftk_la_SOURCES += im/ftk_input_method_hw.c fhw/ftk_handwrite_engine_fhw.c \
	fhw/fhw_primitive_recognizer.c
endif

libftk_la_CFLAGS=

if ENABLE_CAIRO
libftk_la_SOURCES+=ftk_cairo.c
libftk_la_SOURCES += im/ftk_stroke_painter_cairo.c
endif

if ENABLE_TSLIB
LINUX_NATIVE+=$(srcdir)/backend/fb/ftk_source_tslib.c 
endif

if FONT_ENGINE_FREETYPE
libftk_la_SOURCES+=ftk_font_freetype.c
endif

if FONT_ENGINE_DEFAULT
libftk_la_SOURCES+=fontdata.c ftk_font_default.c
endif

if FTK_BACKEND_FB
libftk_la_SOURCES+=$(LINUX_NATIVE) $(OS_LINUX)
endif

if FTK_BACKEND_X11
libftk_la_SOURCES+=$(LINUX_X11) $(OS_LINUX)
endif

if FTK_BACKEND_DFB
libftk_la_SOURCES+=$(LINUX_DFB) $(OS_LINUX)
endif

if FTK_BACKEND_SIGMA
libftk_la_SOURCES+=$(LINUX_SIGMA) $(OS_LINUX)
libftk_la_CFLAGS+=-I$(abs_top_srcdir)/src/backend/sigma
libftk_la_CFLAGS+=-I$(abs_top_srcdir)/src/backend/fb
endif

if FTK_BACKEND_PSP
libftk_la_SOURCES+=$(PSP) $(OS_PSP)
libftk_la_CFLAGS+=-I$(abs_top_srcdir)/src/backend/psp
endif

if FONT_ENGINE_FREETYPE
libftk_la_CFLAGS+=-DFTK_FONT="\"font.ttf\"" 
endif

if FONT_ENGINE_DEFAULT
libftk_la_CFLAGS+=-DFTK_FONT="\"unicode.fnt\"" 
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
	ftk_gesture_listener.h    \
	ftk_globals.h             ftk_text_buffer.h\
	ftk.h                     ftk_typedef.h\
	ftk_icon_cache.h          ftk_util.h\
	ftk_image_bmp_decoder.h   ftk_wait_box.h\
	ftk_image_decoder.h       ftk_widget.h\
	ftk_image.h               \
	ftk_image_jpeg_decoder.h  ftk_window.h\
	ftk_image_png_decoder.h   ftk_wnd_manager_default.h\
	ftk_key.h                 ftk_wnd_manager.h\
	ftk_label.h               ftk_xml_builder.h\
	ftk_list_model_default.h  ftk_xml_parser.h\
	ftk_list_model.h          ftk_xul.h \
	ftk_font_freetype.h       ftk_icon_view.h\
	ftk_theme.h               ftk_painter.h \
	ftk_text_view.h           ftk_cairo.h \
	ftk_pipe.h                ftk_message_box.h \
	ftk_popup_menu.h          ftk_animator_alpha.h \
	ftk_combo_box.h           ftk_clipboard.h \
	ftk_input_method_manager.h ftk_input_method.h \
	ftk_allocator.h            ftk_allocator_default.h \
	ftk_allocator_profile.h    ftk_input_method_preeditor.h \
	im/ftk_input_method_util.h fhw/fhw_primitive_recognizer.h \
	ftk_animator_zoom.h ftk_display_mem.h

EXTRA_DIST=	rules.mk ftk_image_bmp_decoder.c  \
	ftk_image_bmp_decoder.h ftk_input_method_preeditor_default.h \
	Android-build.sh Android.mk


