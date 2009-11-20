INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DFTK_FONT="\"unicode.fnt\"" -DDATA_DIR="\"$(datadir)/ftk\"" \
	-DLINUX -I$(abs_top_srcdir)/src -DTESTDATA_DIR="\"$(abs_top_srcdir)/src/testdata\""
MYLDFLAGS=$(abs_top_builddir)/src/.libs/libftk.a -lpng -ljpeg

noinst_PROGRAMS=demo_label demo_button demo_image demo_image_button demo_dialog demo_multi_win \
	demo_menu demo_entry demo_expand demo_progress_bar demo_check_button demo_wait_box \
	demo_scroll_bar

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

