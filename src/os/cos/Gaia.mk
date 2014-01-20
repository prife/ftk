LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libftk

#-DUSE_LINEBREAK
LOCAL_MODULE_TAGS := eng

#LOCAL_STATIC_LIBRARIES := libpng
#TARGET_PRELINK_MODULES := false
DISPLAY_ENGINE = EGL_FB
FONT_ENGINE    = DEFAULT
#FONT_ENGINE    = FREETYPE
IM_ENGINE      = NONE


FTK_ROOT       = $(LOCAL_PATH)/../../../
ftk_datadir   := /cache
LOCAL_CFLAGS :=	\
	-z \
	-O2 \
	-DHAS_PNG \
	-DHAS_JPEG \
	-DHAS_BMP \
	-DFTK_SUPPORT_C99 \
	-DFTK_SHARED_CANVAS \
	-DFTK_HAS_DLOPEN \
	                \
	-DHAVE_CONFIG_H \
	-DLINUX \
	-DHAVE_CONFIG_FTK_H \
	-DUSE_LINEBREAK \
	                \
	-DFTK_ROOT_DIR="\"$(ftk_datadir)/ftk\"" \
	-DDATA_DIR="\"$(ftk_datadir)/ftk\"" \
	-DFTK_DATA_ROOT="\"$(ftk_datadir)/ftk\"" \
	-DFTK_CNF="\"$(ftk_datadir)/ftk/ftk.cnf\"" \
	-DTESTDATA_DIR="\"$(ftk_datadir)/ftk/testdata\"" -DPNG_ERROR_TEXT_SUPPORTED \
	-DLOCAL_DATA_DIR=\"./local_data\" \
	                \
	-I$(LOCAL_PATH)/../../../src \
	-I$(LOCAL_PATH)/../../../src/im \
	-I$(LOCAL_PATH)/../../../src/os/cos \
	-I$(LOCAL_PATH)/../../../src/canvas \
	-I$(LOCAL_PATH)/../../../src/linebreak \
        -I$(BSP_SYSTEM_DIR)/core/include \
        -I$(BSP_FRAMEWORKS_DIR)/base/include \
        -I$(COMMON_EXTERNAL_DIR)/zlib \
        -I$(COMMON_EXTERNAL_DIR)/libpng \
        -I$(COMMON_EXTERNAL_DIR)/jpeg\

ifeq ($(IM_ENGINE), PINYIN)
	LOCAL_CFLAGS += -DUSE_GPINYIN
endif

ifeq ($(FONT_ENGINE),FREETYPE)
	LOCAL_CFLAGS += -DFTK_FONT="\"font.ttf\"" -DUSE_FREETYPE - I/usr/include/freetype2
else
	LOCAL_CFLAGS += -DFTK_FONT="\"gb2312.fnt\"" -DUSE_DEFAULT_FONT
endif

#ifeq ($(DISPLAY_ENGINE), EGL)
#	CFLAGS += -DUSE_LINUX_FB -DUSE_LINUX_NATIVE
#endif

#FTK_SOURCE += $(wildcard $(LOCAL_PATH)/../../../src/*.c)
FTK_SOURCE += \
	$(FTK_ROOT)/src/ftk_allocator.c \
	$(FTK_ROOT)/src/ftk_allocator_default.c \
	$(FTK_ROOT)/src/ftk_allocator_profile.c \
	$(FTK_ROOT)/src/ftk_animation_alpha.c \
	$(FTK_ROOT)/src/ftk_animation.c \
	$(FTK_ROOT)/src/ftk_animation_expand.c \
	$(FTK_ROOT)/src/ftk_animation_scale.c \
	$(FTK_ROOT)/src/ftk_animation_translate.c \
	$(FTK_ROOT)/src/ftk_animation_trigger_default.c \
	$(FTK_ROOT)/src/ftk_animation_trigger_silence.c \
	$(FTK_ROOT)/src/ftk_app_window.c \
	$(FTK_ROOT)/src/ftk_bitmap.c \
	$(FTK_ROOT)/src/ftk_button.c \
	$(FTK_ROOT)/src/ftk.c \
	$(FTK_ROOT)/src/ftk_canvas.c \
	$(FTK_ROOT)/src/ftk_check_button.c \
	$(FTK_ROOT)/src/ftk_clipboard.c \
	$(FTK_ROOT)/src/ftk_combo_box.c \
	$(FTK_ROOT)/src/ftk_config.c \
	$(FTK_ROOT)/src/ftk_dialog.c \
	$(FTK_ROOT)/src/ftk_display.c \
	$(FTK_ROOT)/src/ftk_display_mem.c \
	$(FTK_ROOT)/src/ftk_display_rotate.c \
	$(FTK_ROOT)/src/ftk_dlfcn.c \
	$(FTK_ROOT)/src/ftk_entry.c \
	$(FTK_ROOT)/src/ftk_expr.c \
	$(FTK_ROOT)/src/ftk_file_browser.c \
	$(FTK_ROOT)/src/ftk_file_system.c \
	$(FTK_ROOT)/src/ftk_file_system_posix.c \
	$(FTK_ROOT)/src/ftk_font_desc.c \
	$(FTK_ROOT)/src/ftk_gesture.c \
	$(FTK_ROOT)/src/ftk_globals.c \
	$(FTK_ROOT)/src/ftk_group_box.c \
	$(FTK_ROOT)/src/ftk_icon_cache.c \
	$(FTK_ROOT)/src/ftk_icon_view.c \
	$(FTK_ROOT)/src/ftk_image.c \
	$(FTK_ROOT)/src/ftk_input_method_chooser_default.c \
	$(FTK_ROOT)/src/ftk_input_method_manager.c \
	$(FTK_ROOT)/src/ftk_input_method_preeditor.c \
	$(FTK_ROOT)/src/ftk_input_method_preeditor_default.c \
	$(FTK_ROOT)/src/ftk_input_pattern.c \
	$(FTK_ROOT)/src/ftk_interpolator_acc_decelerate.c \
	$(FTK_ROOT)/src/ftk_interpolator_accelerate.c \
	$(FTK_ROOT)/src/ftk_interpolator_bounce.c \
	$(FTK_ROOT)/src/ftk_interpolator_decelerate.c \
	$(FTK_ROOT)/src/ftk_interpolator_linear.c \
	$(FTK_ROOT)/src/ftk_key_board.c \
	$(FTK_ROOT)/src/ftk_label.c \
	$(FTK_ROOT)/src/ftk_list_model_default.c \
	$(FTK_ROOT)/src/ftk_list_render_default.c \
	$(FTK_ROOT)/src/ftk_list_view.c \
	$(FTK_ROOT)/src/ftk_log.c \
	$(FTK_ROOT)/src/ftk_main_loop.c \
	$(FTK_ROOT)/src/ftk_main_loop_select.c \
	$(FTK_ROOT)/src/ftk_menu_item.c \
	$(FTK_ROOT)/src/ftk_menu_panel.c \
	$(FTK_ROOT)/src/ftk_message_box.c \
	$(FTK_ROOT)/src/ftk_painter.c \
	$(FTK_ROOT)/src/ftk_pairs.c \
	$(FTK_ROOT)/src/ftk_params.c \
	$(FTK_ROOT)/src/ftk_path.c \
	$(FTK_ROOT)/src/ftk_pipe_socket.c \
	$(FTK_ROOT)/src/ftk_popup_menu.c \
	$(FTK_ROOT)/src/ftk_progress_bar.c \
	$(FTK_ROOT)/src/ftk_scroll_bar.c \
	$(FTK_ROOT)/src/ftk_source_idle.c \
	$(FTK_ROOT)/src/ftk_source_primary.c \
	$(FTK_ROOT)/src/ftk_sources_manager.c \
	$(FTK_ROOT)/src/ftk_source_timer.c \
	$(FTK_ROOT)/src/ftk_sprite.c \
	$(FTK_ROOT)/src/ftk_status_item.c \
	$(FTK_ROOT)/src/ftk_status_panel.c \
	$(FTK_ROOT)/src/ftk_tab.c \
	$(FTK_ROOT)/src/ftk_text_buffer.c \
	$(FTK_ROOT)/src/ftk_text_layout_normal.c \
	$(FTK_ROOT)/src/ftk_text_view.c \
	$(FTK_ROOT)/src/ftk_theme.c \
	$(FTK_ROOT)/src/ftk_translator.c \
	$(FTK_ROOT)/src/ftk_util.c \
	$(FTK_ROOT)/src/ftk_wait_box.c \
	$(FTK_ROOT)/src/ftk_widget.c \
	$(FTK_ROOT)/src/ftk_window.c \
	$(FTK_ROOT)/src/ftk_wnd_manager.c \
	$(FTK_ROOT)/src/ftk_wnd_manager_default.c \
	$(FTK_ROOT)/src/ftk_xml_parser.c \
	$(FTK_ROOT)/src/ftk_xul.c

FTK_SOURCE += $(wildcard $(LOCAL_PATH)/../../../src/linebreak/*.c)

#$(warning "AA"$(FTK_SOURCE))

ifeq ($(IM_ENGINE), PINYIN)
	FTK_SOURCE += $(wildcard $(LOCAL_PATH)/../../../src/im/gpinyin/share/*.cpp)
	FTK_SOURCE += $(wildcard $(LOCAL_PATH)/../../../src/im/gpinyin/ftk/*.cpp)
else
	FTK_SOURCE += $(wildcard $(LOCAL_PATH)/../../../src/im/default/ftk_input_method_util.c)
endif

ifeq ($(DISPLAY_ENGINE), EGL_FB)
	FTK_SOURCE += $(wildcard $(LOCAL_PATH)/*.c)
endif

ifeq ($(FONT_ENGINE), FREETYPE)
	FTK_SOURCE += $(FTK_ROOT)/src/canvas/default/ftk_font_freetype.c
else
	FTK_SOURCE += $(FTK_ROOT)/src/canvas/default/ftk_font_default.c \
				  $(FTK_ROOT)/src/canvas/default/fontdata.c
endif

#add canvas
FTK_SOURCE += $(FTK_ROOT)/src/canvas/default/ftk_bitmap_default.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_bitmap_factory.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_image_bmp_decoder.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_image_jpeg_decoder.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_image_png_decoder.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_font_manager.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_font.c \
			  $(FTK_ROOT)/src/canvas/default/ftk_canvas_default.c

LOCAL_SHARED_LIBRARIES := \
	libdl \
	libjpeg \
	libpng \
	libEGL \
	libGLESv1_CM \
	libcutils
LOCAL_SRC_FILES += $(FTK_SOURCE:$(LOCAL_PATH)/%=%)

#LOCAL_C_INCLUDES := \
#	$(LOCAL_PATH)/../ftkapp  $(JNI_H_INCLUDE)

#LOCAL_SRC_FILES := $(LOCAL_SRC_FILES)

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
