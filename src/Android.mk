LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	ftk_app_window.c      ftk_icon_cache.c          ftk_sources_manager.c\
	ftk_bitmap.c          ftk_image.c               ftk_source_timer.c\
	ftk_bitmap_factory.c  ftk_image_jpeg_decoder.c  \
	ftk_button.c          ftk_image_png_decoder.c   ftk_status_item.c\
	ftk.c                 ftk_label.c               ftk_status_panel.c\
	ftk_canvas.c          ftk_log.c                 ftk_style.c\
	ftk_dialog.c          ftk_main_loop.c           ftk_test.c\
	ftk_display_fb.c      ftk_menu_item.c           ftk_text_buffer.c\
	ftk_menu_panel.c      ftk_util.c\
	ftk_entry.c           ftk_source_idle.c         ftk_widget.c\
	ftk_font.c            ftk_source_input.c        ftk_window.c\
	ftk_globals.c         ftk_source_primary.c      ftk_wnd_manager_default.c \
	fontdata.c

LOCAL_CFLAGS += -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"./ftk\"" \
	-DFTK_FB_NAME="\"/dev/graphics/fb0\""
LOCAL_C_INCLUDES := external/jpeg external/libpng external/zlib
LOCAL_MODULE:=libftk
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= demo_button.c
LOCAL_MODULE := demo_button
LOCAL_STATIC_LIBRARIES := libcutils libc libftk libjpeg libpng libz
include $(BUILD_EXECUTABLE)

