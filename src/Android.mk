LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	fontdata.c                 ftk_list_view.c\
	ftk_animator_expand.c      ftk_log.c\
	ftk_app_window.c           ftk_main_loop.c\
	ftk_bitmap.c               ftk_menu_item.c\
	ftk_bitmap_factory.c       ftk_menu_panel.c\
	ftk_button.c               ftk_progress_bar.c\
	ftk.c                      ftk_radio_group.c\
	ftk_canvas.c               ftk_scroll_bar.c\
	ftk_check_button.c         ftk_source_idle.c\
	ftk_dialog.c               ftk_source_primary.c\
	ftk_display.c              ftk_sources_manager.c\
	ftk_entry.c                ftk_source_timer.c\
	ftk_expr.c                 ftk_sprite.c\
	ftk_font_default.c         ftk_status_item.c\
	ftk_status_panel.c\
	ftk_gesture.c              \
	ftk_globals.c              ftk_text_buffer.c\
	ftk_icon_cache.c           ftk_util.c\
	ftk_image_bmp_decoder.c    ftk_wait_box.c\
	ftk_image.c                ftk_widget.c\
	ftk_image_jpeg_decoder.c   ftk_window.c\
	ftk_image_png_decoder.c    ftk_wnd_manager_default.c\
	ftk_label.c                ftk_xml_parser.c\
	ftk_list_model_default.c   ftk_xul.c\
	ftk_theme.c \
	ftk_list_render_default.c\
	backend/fb/ftk_source_input.c \
	backend/fb/ftk_display_fb.c \
	backend/fb/ftk_backend_fb.c \
	os/linux/ftk_mmap_linux.c \
	os/linux/ftk_linux.c \
	ftk_icon_view.c

LOCAL_CFLAGS += -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"/data/ftk/base\"" \
	-DFTK_FB_NAME="\"/dev/graphics/fb0\"" -DUSE_LINUX_NATIVE -DLOCAL_DATA_DIR="\"./ftk/base\""\
	-DFTK_DATA_ROOT="\"/data/ftk\"" -DTESTDATA_DIR="\"/data/ftk/testdata\"" \
	-I$(LOCAL_PATH)/os/linux -I$(LOCAL_PATH)/backend/fb -DLINUX -DFTK_SUPPORT_C99

LOCAL_C_INCLUDES := external/jpeg external/libpng external/zlib 
LOCAL_MODULE:=libftk
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS+=-DTESTDATA_DIR="\"./ftk/testdata\""
LOCAL_SRC_FILES:= demos/demo_xul.c
LOCAL_MODULE := demo_xul
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demos/demo_button.c
LOCAL_MODULE := demo_button
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demos/demo_label.c
LOCAL_MODULE := demo_label
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demos/demo_entry.c
LOCAL_MODULE := demo_entry
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demos/demo_dialog.c
LOCAL_MODULE := demo_dialog
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demos/demo_menu.c
LOCAL_MODULE := demo_menu
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS+=-DTESTDATA_DIR="\"./ftk/testdata\""
LOCAL_SRC_FILES := demos/demo_listview.c
LOCAL_MODULE := demo_listview
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := demos/demo_progress_bar.c
LOCAL_MODULE := demo_progress_bar
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS+=-DTESTDATA_DIR="\"./ftk/testdata\""
LOCAL_SRC_FILES := demos/demo_transparent.c
LOCAL_MODULE := demo_transparent
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := demos/demo_sprite.c
LOCAL_MODULE := demo_sprite
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

