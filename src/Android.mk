LOCAL_PATH:= $(call my-dir)
FTK_CFLAGS :=	-DHAS_PNG -DHAS_JPEG -DHAS_BMP -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"/data/ftk/base\"" \
	-DFTK_FB_NAME="\"/dev/graphics/fb0\"" -DUSE_LINUX_NATIVE -DLOCAL_DATA_DIR="\"./ftk/base\""\
	-DFTK_DATA_ROOT="\"/data/ftk\"" -DTESTDATA_DIR="\"/data/ftk/testdata\"" \
	-I$(LOCAL_PATH)/os/linux -I$(LOCAL_PATH)/backend/fb -DFTK_SUPPORT_C99 \
	-I$(LOCAL_PATH)/im -DLINUX \
	-DFTK_CNF="\"/data/ftk/ftk.cnf\"" 

include $(CLEAR_VARS)
OS_LINUX=\
	$(srcdir)/os/linux/ftk_mmap_linux.c  \
	$(srcdir)/os/linux/ftk_linux.c 
LINUX_NATIVE=$(srcdir)/backend/fb/ftk_display_fb.c \
	$(srcdir)/backend/fb/ftk_source_input.c \
	$(srcdir)/backend/fb/ftk_backend_fb.c
LOCAL_SRC_FILES := $(OS_LINUX) $(LINUX_NATIVE) \
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
	ftk_main_loop_select.c ftk_config.c \
	fontdata.c ftk_font_default.c

LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_C_INCLUDES := external/jpeg external/libpng external/zlib 
LOCAL_MODULE:=libftk
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_xul.c
LOCAL_MODULE := demo_xul
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_button.c
LOCAL_MODULE := demo_button
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_label.c
LOCAL_MODULE := demo_label
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_entry.c
LOCAL_MODULE := demo_entry
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_dialog.c
LOCAL_MODULE := demo_dialog
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES:= demos/demo_menu.c
LOCAL_MODULE := demo_menu
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_listview.c
LOCAL_MODULE := demo_listview
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_progress_bar.c
LOCAL_MODULE := demo_progress_bar
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_transparent.c
LOCAL_MODULE := demo_transparent
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_sprite.c
LOCAL_MODULE := demo_sprite
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_combo_box.c
LOCAL_MODULE := demo_combo_box
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_zoom.c
LOCAL_MODULE := demo_zoom
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_alpha.c
LOCAL_MODULE := demo_alpha
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_popup.c
LOCAL_MODULE := demo_popup
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += $(FTK_CFLAGS)
LOCAL_SRC_FILES := demos/demo_msgbox.c
LOCAL_MODULE := demo_msgbox
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

