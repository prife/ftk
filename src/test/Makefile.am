include $(abs_top_srcdir)/src/rules.mk

noinst_PROGRAMS=label_test widget_test \
	canvas_test ftk_test bitmap_test font_test primary_source_test\
	bitmap_factory_test main_loop_test png_test log_test util_test \
	sources_manager_test text_buffer_test icon_cache_test  \
	xml_parser_test xul_test list_model_default_test timer_test \
	expr_test theme_test xul_stress_test clipboard_test bmp_test \
	lookup_table_test config_test fs_test tab_test font_desc_test \
	font_manager_test pairs_test params_test animation_trigger_default_test \
	interpolator_test

fs_test_SOURCES=fs_test.c
fs_test_LDFLAGS=$(MYLDFLAGS)

tab_test_SOURCES=tab_test.c
tab_test_LDFLAGS=$(MYLDFLAGS)

bin_PROGRAMS=display_test
display_test_SOURCES=display_test.c
display_test_LDFLAGS=$(MYLDFLAGS)
display_test_CFLAGS=-I$(abs_top_srcdir)/src/backend/native

if FTK_BACKEND_SIGMA
noinst_PROGRAMS+=backend_sigma_test
backend_sigma_test_SOURCES=backend_sigma_test.c
backend_sigma_test_LDFLAGS=$(MYLDFLAGS)
backend_sigma_test_CFLAGS=-I$(abs_top_srcdir)/src/backend/sigma
endif

if FTK_BACKEND_FB
noinst_PROGRAMS +=input_test

input_test_SOURCES=input_test.c
input_test_LDFLAGS=$(MYLDFLAGS)
input_test_CFLAGS=-I$(abs_top_srcdir)/src/backend/native
if ENABLE_TSLIB
noinst_PROGRAMS +=tslib_test
tslib_test_SOURCES=tslib_test.c
tslib_test_LDFLAGS=$(MYLDFLAGS)
tslib_test_CFLAGS=-I$(abs_top_srcdir)/src/backend/native
endif
endif

config_test_SOURCES=config_test.c
config_test_LDFLAGS=$(MYLDFLAGS)

lookup_table_test_SOURCES=lookup_table_test.c
lookup_table_test_LDFLAGS=$(MYLDFLAGS)

bmp_test_SOURCES=bmp_test.c
bmp_test_LDFLAGS=$(MYLDFLAGS)

clipboard_test_SOURCES=clipboard_test.c
clipboard_test_LDFLAGS=$(MYLDFLAGS)

xul_stress_test_SOURCES=xul_stress_test.c
xul_stress_test_LDFLAGS=$(MYLDFLAGS)

theme_test_SOURCES=theme_test.c
theme_test_LDFLAGS=$(MYLDFLAGS)

expr_test_SOURCES=expr_test.c
expr_test_LDFLAGS=$(MYLDFLAGS)

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

font_desc_test_SOURCES=font_desc_test.c
font_desc_test_LDFLAGS=$(MYLDFLAGS)

font_manager_test_SOURCES=font_manager_test.c
font_manager_test_LDFLAGS=$(MYLDFLAGS)

pairs_test_SOURCES=pairs_test.c
pairs_test_LDFLAGS=$(MYLDFLAGS)

params_test_SOURCES=params_test.c
params_test_LDFLAGS=$(MYLDFLAGS)

animation_trigger_default_test_SOURCES=animation_trigger_default_test.c
animation_trigger_default_test_LDFLAGS=$(MYLDFLAGS)

interpolator_test_SOURCES=interpolator_test.c
interpolator_test_LDFLAGS=$(MYLDFLAGS)

EXTRA_DIST=test_util.c xml_builder_dump.h
