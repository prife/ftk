LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libftk

#-DUSE_LINEBREAK

LOCAL_CFLAGS :=	\
	-O2 \
	-DANDROID_NDK \
	-DUSE_FREETYPE \
	-DFTK_PROFILE \
	-DFTK_FONT=\"/system/fonts/DroidSansFallback.ttf\" \
	-DDATA_DIR=\"/assets\" \
	-DLOCAL_DATA_DIR=\"./local_data\" \
	-DFTK_DATA_ROOT=\"/assets\" \
	-DTESTDATA_DIR=\"/assets/testdata\" \
	-I$(LOCAL_PATH)/../../../../../src \
	-I$(LOCAL_PATH)/../../../../../src/im \
	-I$(LOCAL_PATH)/../../../../../src/backend/android \
	-I$(LOCAL_PATH)/../../../../../src/os/android

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../ftkapp \
	$(LOCAL_PATH)/../freetype/include

LOCAL_SRC_FILES := \
	../../../../../src/ftk_allocator.c \
	../../../../../src/ftk_allocator_default.c \
	../../../../../src/ftk_allocator_profile.c \
	../../../../../src/ftk_app_window.c \
	../../../../../src/ftk_bitmap.c \
	../../../../../src/ftk_bitmap_factory.c \
	../../../../../src/ftk_button.c \
	../../../../../src/ftk.c \
	../../../../../src/ftk_animation_alpha.c \
	../../../../../src/ftk_animation.c \
	../../../../../src/ftk_animation_expand.c \
	../../../../../src/ftk_animation_scale.c \
	../../../../../src/ftk_animation_translate.c \
	../../../../../src/ftk_animation_trigger_default.c \
	../../../../../src/ftk_animation_trigger_silence.c \
	../../../../../src/ftk_interpolator_acc_decelerate.c \
	../../../../../src/ftk_interpolator_accelerate.c \
	../../../../../src/ftk_interpolator_bounce.c \
	../../../../../src/ftk_interpolator_decelerate.c \
	../../../../../src/ftk_interpolator_linear.c \
	../../../../../src/ftk_pairs.c \
	../../../../../src/ftk_params.c \
	../../../../../src/ftk_canvas.c \
	../../../../../src/ftk_check_button.c \
	../../../../../src/ftk_clipboard.c \
	../../../../../src/ftk_combo_box.c \
	../../../../../src/ftk_config.c \
	../../../../../src/ftk_dialog.c \
	../../../../../src/ftk_display.c \
	../../../../../src/ftk_display_mem.c \
	../../../../../src/ftk_display_rotate.c \
	../../../../../src/ftk_entry.c \
	../../../../../src/ftk_expr.c \
	../../../../../src/ftk_file_browser.c \
	../../../../../src/ftk_file_system.c \
	../../../../../src/ftk_file_system_posix.c \
	../../../../../src/ftk_font.c \
	../../../../../src/ftk_font_desc.c \
	../../../../../src/ftk_font_manager.c \
	../../../../../src/ftk_font_freetype.c \
	../../../../../src/ftk_gesture.c \
	../../../../../src/ftk_globals.c \
	../../../../../src/ftk_icon_cache.c \
	../../../../../src/ftk_icon_view.c \
	../../../../../src/ftk_image.c \
	../../../../../src/ftk_input_method_manager.c \
	../../../../../src/ftk_input_method_preeditor.c \
	../../../../../src/ftk_input_method_preeditor_default.c \
	../../../../../src/ftk_label.c \
	../../../../../src/ftk_list_model_default.c \
	../../../../../src/ftk_list_render_default.c \
	../../../../../src/ftk_list_view.c \
	../../../../../src/ftk_log.c \
	../../../../../src/ftk_main_loop.c \
	../../../../../src/ftk_main_loop_select.c \
	../../../../../src/ftk_menu_item.c \
	../../../../../src/ftk_menu_panel.c \
	../../../../../src/ftk_message_box.c \
	../../../../../src/ftk_painter.c \
	../../../../../src/ftk_path.c \
	../../../../../src/ftk_pipe_socket.c \
	../../../../../src/ftk_popup_menu.c \
	../../../../../src/ftk_progress_bar.c \
	../../../../../src/ftk_radio_group.c \
	../../../../../src/ftk_scroll_bar.c \
	../../../../../src/ftk_source_idle.c \
	../../../../../src/ftk_source_primary.c \
	../../../../../src/ftk_sources_manager.c \
	../../../../../src/ftk_source_timer.c \
	../../../../../src/ftk_sprite.c \
	../../../../../src/ftk_status_item.c \
	../../../../../src/ftk_status_panel.c \
	../../../../../src/ftk_tab.c \
	../../../../../src/ftk_text_buffer.c \
	../../../../../src/ftk_text_layout_normal.c \
	../../../../../src/ftk_text_view.c \
	../../../../../src/ftk_theme.c \
	../../../../../src/ftk_translator.c \
	../../../../../src/ftk_util.c \
	../../../../../src/ftk_wait_box.c \
	../../../../../src/ftk_widget.c \
	../../../../../src/ftk_window.c \
	../../../../../src/ftk_wnd_manager.c \
	../../../../../src/ftk_wnd_manager_default.c \
	../../../../../src/ftk_xml_parser.c \
	../../../../../src/ftk_xul.c \
	../../../../../src/ftk_canvas_default.c \
	../../../../../src/backend/android/ftk_backend_android.c \
	../../../../../src/backend/android/ftk_display_android_skia.c \
	../../../../../src/backend/android/ftk_source_android.c \
	../../../../../src/os/android/ftk_android.c \
	../../../../../src/os/android/ftk_image_android_decoder.c \
	../../../../../src/os/android/ftk_input_method_android.c \
	../../../../../src/os/android/ftk_input_method_chooser_android.c \
	../../../../../src/os/android/ftk_mmap_android.c

include $(BUILD_STATIC_LIBRARY)
