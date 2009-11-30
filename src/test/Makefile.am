include $(abs_top_srcdir)/src/rules.mk

noinst_PROGRAMS=label_test widget_test \
	canvas_test ftk_test bitmap_test font_test primary_source_test\
	bitmap_factory_test main_loop_test png_test log_test util_test \
	sources_manager_test text_buffer_test icon_cache_test  \
	xml_parser_test xul_test list_model_default_test timer_test

#if FTK_BACKEND_FB
#INCLUDES=$(INCLUDES)
#INCLUDES+=-I$(abs_top_srcdir)/src/backend/fb
#noinst_PROGRAMS +=display_fb_test input_test
#display_fb_test_SOURCES=display_fb_test.c
#display_fb_test_LDFLAGS=$(MYLDFLAGS)
#input_test_SOURCES=input_test.c
#input_test_LDFLAGS=$(MYLDFLAGS)
#endif

timer_test_SOURCES=timer_test.c
timer_test_LDFLAGS=$(MYLDFLAGS)

list_model_default_test_SOURCES=list_model_default_test.c
list_model_default_test_LDFLAGS=$(MYLDFLAGS)

xul_test_SOURCES=xul_test.c
xul_test_LDFLAGS=$(MYLDFLAGS)

xml_parser_test_SOURCES=xml_builder_dump.c  xml_parser_test.c
xml_parser_test_LDFLAGS=$(MYLDFLAGS)

icon_cache_test_SOURCES=icon_cache_test.c
icon_cache_test_LDFLAGS=$(MYLDFLAGS)

sources_manager_test_SOURCES=sources_manager_test.c
sources_manager_test_LDFLAGS=$(MYLDFLAGS)

text_buffer_test_SOURCES=text_buffer_test.c
text_buffer_test_LDFLAGS=$(MYLDFLAGS)

util_test_SOURCES=util_test.c
util_test_LDFLAGS=$(MYLDFLAGS)

log_test_SOURCES=log_test.c
log_test_LDFLAGS=$(MYLDFLAGS)

png_test_SOURCES=png_test.c
png_test_LDFLAGS=$(MYLDFLAGS)

font_test_SOURCES=font_test.c
font_test_LDFLAGS=$(MYLDFLAGS)

bitmap_test_SOURCES=bitmap_test.c
bitmap_test_LDFLAGS=$(MYLDFLAGS)

label_test_SOURCES=label_test.c
label_test_LDFLAGS=$(MYLDFLAGS)

widget_test_SOURCE=widget_test.c
widget_test_LDFLAGS=$(MYLDFLAGS)

canvas_test_SOURCES=canvas_test.c
canvas_test_LDFLAGS=$(MYLDFLAGS)

ftk_test_SOURCES=ftk_test.c
ftk_test_LDFLAGS=$(MYLDFLAGS)

bitmap_factory_test_SOURCES=bitmap_factory_test.c
bitmap_factory_test_LDFLAGS=$(MYLDFLAGS)

main_loop_test_SOURCES=main_loop_test.c
main_loop_test_LDFLAGS=$(MYLDFLAGS)

primary_source_test_SOURCES=primary_source_test.c
primary_source_test_LDFLAGS=$(MYLDFLAGS)

EXTRA_DIST=test_util.c xml_builder_dump.h
