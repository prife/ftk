include $(abs_top_srcdir)/src/rules.mk

lib_LTLIBRARIES=libapp-demo_alpha.la libapp-demo_button.la libapp-demo_check_button.la libapp-demo_combo_box.la libapp-demo_dialog.la libapp-demo_entry.la libapp-demo_expand.la libapp-demo_fullscreen.la libapp-demo_hello.la libapp-demo_icon_view.la libapp-demo_image_button.la libapp-demo_image.la libapp-demo_ime.la libapp-demo_label.la libapp-demo_listview.la libapp-demo_menu.la libapp-demo_msgbox.la libapp-demo_multi_win.la libapp-demo_popup.la libapp-demo_progress_bar.la libapp-demo_scroll_bar.la libapp-demo_sprite.la libapp-demo_text_view.la libapp-demo_transparent.la libapp-demo_wait_box.la libapp-demo_xul.la libapp-demo_zoom.la
noinst_PROGRAMS=demo_label demo_button demo_image demo_image_button demo_dialog demo_multi_win \
	demo_menu demo_entry demo_expand demo_zoom demo_progress_bar demo_check_button demo_wait_box \
	demo_scroll_bar demo_sprite demo_xul demo_transparent demo_listview demo_hello \
	demo_fullscreen demo_icon_view demo_text_view demo_msgbox demo_popup demo_alpha \
	demo_combo_box demo_ime demo_statusbar

if ENABLE_CAIRO
noinst_PROGRAMS+=demo_cairo
demo_cairo_SOURCES=demo_cairo.c
demo_cairo_LDFLAGS=$(MYLDFLAGS)

lib_LTLIBRARIES += libapp-demo_cairo.la
libapp_demo_cairo_la_SOURCES=demo_cairo.c
libapp_demo_cairo_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_cairo_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

endif

demo_statusbar_SOURCES=demo_statusbar.c
demo_statusbar_LDFLAGS=$(MYLDFLAGS)

demo_ime_SOURCES=demo_ime.c
demo_ime_LDFLAGS=$(MYLDFLAGS)

demo_combo_box_SOURCES=demo_combo_box.c
demo_combo_box_LDFLAGS=$(MYLDFLAGS)

demo_alpha_SOURCES=demo_alpha.c
demo_alpha_LDFLAGS=$(MYLDFLAGS)

demo_zoom_SOURCES=demo_zoom.c
demo_zoom_LDFLAGS=$(MYLDFLAGS)

demo_popup_SOURCES=demo_popup.c
demo_popup_LDFLAGS=$(MYLDFLAGS)

demo_msgbox_SOURCES=demo_msgbox.c
demo_msgbox_LDFLAGS=$(MYLDFLAGS)

demo_text_view_SOURCES=demo_text_view.c
demo_text_view_LDFLAGS=$(MYLDFLAGS)

demo_icon_view_SOURCES=demo_icon_view.c
demo_icon_view_LDFLAGS=$(MYLDFLAGS)

demo_fullscreen_SOURCES=demo_fullscreen.c
demo_fullscreen_LDFLAGS=$(MYLDFLAGS)

demo_hello_SOURCES=demo_hello.c
demo_hello_LDFLAGS=$(MYLDFLAGS)

demo_listview_SOURCES=demo_listview.c
demo_listview_LDFLAGS=$(MYLDFLAGS)

demo_transparent_SOURCES=demo_transparent.c
demo_transparent_LDFLAGS=$(MYLDFLAGS)

demo_xul_SOURCES=demo_xul.c
demo_xul_LDFLAGS=$(MYLDFLAGS)

demo_sprite_SOURCES=demo_sprite.c
demo_sprite_LDFLAGS=$(MYLDFLAGS)

demo_scroll_bar_SOURCES=demo_scroll_bar.c
demo_scroll_bar_LDFLAGS=$(MYLDFLAGS)

demo_wait_box_SOURCES=demo_wait_box.c
demo_wait_box_LDFLAGS=$(MYLDFLAGS)

demo_check_button_SOURCES=demo_check_button.c
demo_check_button_LDFLAGS=$(MYLDFLAGS)

demo_progress_bar_SOURCES=demo_progress_bar.c
demo_progress_bar_LDFLAGS=$(MYLDFLAGS)

demo_expand_SOURCES=demo_expand.c
demo_expand_LDFLAGS=$(MYLDFLAGS)

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

##################################################

libapp_demo_alpha_la_SOURCES=demo_alpha.c
libapp_demo_alpha_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_alpha_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_button_la_SOURCES=demo_button.c
libapp_demo_button_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_button_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_check_button_la_SOURCES=demo_check_button.c
libapp_demo_check_button_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_check_button_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_combo_box_la_SOURCES=demo_combo_box.c
libapp_demo_combo_box_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_combo_box_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_dialog_la_SOURCES=demo_dialog.c
libapp_demo_dialog_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_dialog_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_entry_la_SOURCES=demo_entry.c
libapp_demo_entry_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_entry_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_expand_la_SOURCES=demo_expand.c
libapp_demo_expand_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_expand_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_fullscreen_la_SOURCES=demo_fullscreen.c
libapp_demo_fullscreen_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_fullscreen_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_hello_la_SOURCES=demo_hello.c
libapp_demo_hello_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_hello_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_icon_view_la_SOURCES=demo_icon_view.c
libapp_demo_icon_view_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_icon_view_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_image_button_la_SOURCES=demo_image_button.c
libapp_demo_image_button_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_image_button_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_image_la_SOURCES=demo_image.c
libapp_demo_image_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_image_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_ime_la_SOURCES=demo_ime.c
libapp_demo_ime_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_ime_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_label_la_SOURCES=demo_label.c
libapp_demo_label_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_label_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_listview_la_SOURCES=demo_listview.c
libapp_demo_listview_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_listview_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_menu_la_SOURCES=demo_menu.c
libapp_demo_menu_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_menu_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_msgbox_la_SOURCES=demo_msgbox.c
libapp_demo_msgbox_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_msgbox_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_multi_win_la_SOURCES=demo_multi_win.c
libapp_demo_multi_win_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_multi_win_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_popup_la_SOURCES=demo_popup.c
libapp_demo_popup_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_popup_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_progress_bar_la_SOURCES=demo_progress_bar.c
libapp_demo_progress_bar_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_progress_bar_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_scroll_bar_la_SOURCES=demo_scroll_bar.c
libapp_demo_scroll_bar_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_scroll_bar_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_sprite_la_SOURCES=demo_sprite.c
libapp_demo_sprite_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_sprite_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_text_view_la_SOURCES=demo_text_view.c
libapp_demo_text_view_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_text_view_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_transparent_la_SOURCES=demo_transparent.c
libapp_demo_transparent_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_transparent_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_wait_box_la_SOURCES=demo_wait_box.c
libapp_demo_wait_box_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_wait_box_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_xul_la_SOURCES=demo_xul.c
libapp_demo_xul_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_xul_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

libapp_demo_zoom_la_SOURCES=demo_zoom.c
libapp_demo_zoom_la_LDFLAGS=$(MYLDFLAGS)
libapp_demo_zoom_la_CFLAGS=$(LOCAL_CFLAGS) -DFTK_AS_PLUGIN

desktopdir=$(datadir)/ftk/base/apps
desktop_DATA=demos.desktop

iconsdir=$(datadir)/ftk/demos/icons
icons_DATA=demo.png

EXTRA_DIST=demo.png test_util.c t1.xul  t2.xul  t3.xul  t4.xul  t5.xul  t6.xul  t7.xul demos.desktop
