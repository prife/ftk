/*
 * File: ftk.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: ftk global init, mainloop and deinit functions.  
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk.h"
#include "ftk_backend.h"
#include "ftk_globals.h"
#include "ftk_main_loop.h"
#include "ftk_status_item.h"
#ifdef USE_FREETYPE
#include "ftk_font_freetype.h"
#else
#include "ftk_font_default.h"
#endif
#include "ftk_status_panel.h"
#include "ftk_bitmap_factory.h"
#include "ftk_wnd_manager_default.h"

static void ftk_deinit(void);
static void ftk_init_panel(void);

static Ret ftk_init_bitmap_factory(void)
{
	ftk_set_bitmap_factory(ftk_bitmap_factory_create());
	
	return RET_OK;
}

static Ret ftk_init_font(void)
{
	FtkFont* font = NULL;
	char filename[FTK_MAX_PATH] = {0};
#ifdef USE_FREETYPE
	ftk_snprintf(filename, sizeof(filename), "%s/data/%s", LOCAL_DATA_DIR, FTK_FONT);
	if((font = ftk_font_freetype_create(filename, 0, 0, FTK_FONT_SIZE)) == NULL)
	{
		ftk_snprintf(filename, sizeof(filename), "%s/data/%s", DATA_DIR, FTK_FONT);
		font = ftk_font_freetype_create(filename, 0, 0, 0);
	}
#else
	ftk_snprintf(filename, sizeof(filename), "%s/data/%s", LOCAL_DATA_DIR, FTK_FONT);
	if((font = ftk_font_default_create(filename, 0, 0, 0)) == NULL)
	{
		ftk_snprintf(filename, sizeof(filename), "%s/data/%s", DATA_DIR, FTK_FONT);
		font = ftk_font_default_create(filename, 0, 0, 0);
	}
#endif
	
	if(font != NULL)
	{
		ftk_set_font(font);
	}
	else
	{
		ftk_deinit();
		ftk_loge("load font failed.\n");
		exit(0);
	}

	return RET_OK;
}

static Ret ftk_init_theme(const char* theme)
{
	char filename[FTK_MAX_PATH] = {0};

	ftk_set_theme(ftk_theme_create(theme == NULL));
	if(theme != NULL)
	{
		ftk_snprintf(filename, sizeof(filename)-1, "%s/theme/%s/theme.xml", DATA_DIR, theme);
		if(ftk_theme_parse_file(ftk_default_theme(), filename) != RET_OK)
		{
			ftk_snprintf(filename, sizeof(filename)-1, "%s/theme/%s/theme.xml", LOCAL_DATA_DIR, theme);
			ftk_theme_parse_file(ftk_default_theme(), filename);
		}
	}

	return RET_OK;
}

static void ftk_deinit(void)
{
	if(ftk_default_sources_manager() != NULL)
	{
		ftk_sources_manager_destroy(ftk_default_sources_manager());
		ftk_set_sources_manager(NULL);
	}

	if(ftk_default_main_loop() != NULL)
	{
		ftk_main_loop_destroy(ftk_default_main_loop());
		ftk_set_main_loop(NULL);
	}

	if(ftk_default_wnd_manager() != NULL)
	{
		ftk_wnd_manager_destroy(ftk_default_wnd_manager());
		ftk_set_wnd_manager(NULL);
	}

	if(ftk_default_bitmap_factory() != NULL)
	{
		ftk_bitmap_factory_destroy(ftk_default_bitmap_factory());
		ftk_set_bitmap_factory(NULL);
	}

	if(ftk_default_font() != NULL)
	{
		ftk_font_destroy(ftk_default_font());
		ftk_set_font(NULL);
	}

	if(ftk_default_display() != NULL)
	{
		ftk_display_destroy(ftk_default_display());
		ftk_set_display(NULL);
	}

	if(ftk_default_theme() != NULL)
	{
		ftk_theme_destroy(ftk_default_theme());
		ftk_set_theme(NULL);
	}

	if(ftk_shared_canvas() != NULL)
	{
		ftk_canvas_destroy(ftk_shared_canvas());
		ftk_set_shared_canvas(NULL);
	}

	ftk_platform_deinit();

	ftk_logd("%s: ftk exit.\n", __func__);
	return;
}

static Ret ftk_move_cursor(void* ctx, void* obj)
{
	FtkEvent* event = obj;

	if(event->type == FTK_EVT_MOUSE_MOVE)
	{
		ftk_sprite_move(ctx, event->u.mouse.x, event->u.mouse.y);
	}

	return RET_OK;
}

static Ret ftk_enable_curosr(void)
{
	FtkSprite* sprite = ftk_sprite_create();
	FtkBitmap* icon = ftk_theme_load_image(ftk_default_theme(), "cursor"FTK_STOCK_IMG_SUFFIX);
	ftk_sprite_set_icon(sprite, icon);
	ftk_sprite_show(sprite, 1);
	ftk_wnd_manager_add_global_listener(ftk_default_wnd_manager(), ftk_move_cursor, sprite);

	return RET_OK;
}

Ret ftk_init(int argc, char* argv[])
{
	int i = 0;
	FtkColor bg = {0};
	const char* theme = NULL;
	FtkDisplay* display = NULL;
	int disable_status_panel = 0;
	int enable_cursor = 0;

	for(i = 0; i < argc && argv != NULL && argv[i] != NULL; i++)
	{
		if(strcmp(argv[i], "--no-status-panel") == 0)
		{
			disable_status_panel = 1;
			break;
		}
		else if(strcmp(argv[i], "--enable-cursor") == 0)
		{
			enable_cursor = 1;
			break;
		}
		else if(strncmp(argv[i], "--theme=", 8) == 0)
		{
			theme = argv[i]+8;
			ftk_logd("theme=%s\n", theme);
			break;
		}
	}

	ftk_platform_init(argc, argv);

	ftk_set_sources_manager(ftk_sources_manager_create(64));
	ftk_set_main_loop(ftk_main_loop_create(ftk_default_sources_manager()));
	ftk_set_wnd_manager(ftk_wnd_manager_default_create(ftk_default_main_loop()));

	ftk_init_font();
	ftk_init_bitmap_factory();
	
	ftk_init_theme(theme);
	ftk_backend_init(argc, argv);

	bg.a = 0xff;
	display = ftk_default_display();
	ftk_set_shared_canvas(ftk_canvas_create(ftk_display_width(display), ftk_display_height(display), bg));

	if(!disable_status_panel)
	{
		ftk_init_panel();
	}

	if(enable_cursor)
	{
		ftk_enable_curosr();
	}

	atexit(ftk_deinit);

	return RET_OK;
}

Ret ftk_run(void)
{
	return ftk_main_loop_run(ftk_default_main_loop());
}

void ftk_quit(void)
{
	if(ftk_default_main_loop() != NULL)
	{
		ftk_main_loop_quit(ftk_default_main_loop());
		ftk_logd("%s\n", __func__);
	}

	return;
}

#define IDC_TITLE_ITEM 1000
#define IDC_ICON_ITEM  1001
#define IDC_CLOSE_ITEM 1002

static Ret on_wnd_manager_global_event(void* ctx, void* obj)
{
	Ret ret = RET_OK;
	FtkEvent* event = obj;

	switch(event->type)
	{
		case FTK_EVT_TOP_WND_CHANGED:
		{
			FtkWidget* panel = ftk_default_status_panel();
			FtkWidget* top_window = event->widget;
			FtkWidget* title_widget = ftk_widget_lookup(panel, IDC_TITLE_ITEM);

			if(top_window != NULL)
			{
				ftk_widget_set_text(title_widget, ftk_widget_get_text(top_window));
				ftk_logd("top_window changed: %s\n", ftk_widget_get_text(top_window));
			}
			else
			{
				ftk_widget_set_text(title_widget, NULL);
			}
			ftk_widget_set_user_data(title_widget, NULL, top_window);
			ret = RET_REMOVE;

			break;
		}
		case FTK_EVT_WND_CONFIG_CHANGED:
		{
			FtkWidget* panel = ftk_default_status_panel();
			FtkWidget* title_widget = ftk_widget_lookup(panel, IDC_TITLE_ITEM);
			FtkWidget* top_window = ftk_widget_user_data(title_widget);
			if(top_window == event->widget)
			{
				ftk_widget_set_text(title_widget, ftk_widget_get_text(top_window));
			}
			
			ftk_logd("%s: config changed: %p %p\n", __func__, top_window, event->widget);
			ret = RET_REMOVE;
			break;
		}
		default:break;
	}

	return ret;
}

static Ret button_close_top_clicked(void* ctx, void* obj)
{
	FtkWidget* panel = ftk_default_status_panel();
	FtkWidget* title_widget = ftk_widget_lookup(panel, IDC_TITLE_ITEM);
	FtkWidget* top_window = ftk_widget_user_data(title_widget);

	if(top_window != NULL && ftk_widget_type(top_window) == FTK_WINDOW)
	{
		if(!ftk_widget_has_attr(top_window, FTK_ATTR_IGNORE_CLOSE))
		{
			ftk_logd("%s: close window %s\n", __func__, ftk_widget_get_text(top_window));
			ftk_widget_unref(top_window);
			ftk_widget_set_user_data(title_widget, NULL, NULL);
		}
		else
		{
			ftk_logd("%s: the top window has attribute FTK_ATTR_IGNORE_CLOSE\n", __func__);
		}
	}
	else
	{
		ftk_logd("%s: not normal window.\n", __func__);
	}

	return RET_OK;
}

static void ftk_init_panel(void)
{
	FtkGc gc = {0};
	FtkWidget* item = NULL;	
	FtkWidget* panel = ftk_status_panel_create(FTK_STATUS_PANEL_HEIGHT);
	
	gc.mask = FTK_GC_BITMAP;
	ftk_set_status_panel(panel);
	gc.bitmap = ftk_theme_load_image(ftk_default_theme(), "status-bg"FTK_STOCK_IMG_SUFFIX);
	ftk_widget_set_gc(panel, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(panel, FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(panel, FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	
	item = ftk_status_item_create(panel, -1, 32);
	ftk_widget_set_id(item, IDC_CLOSE_ITEM);
	gc.bitmap = ftk_theme_load_image(ftk_default_theme(), "close-32"FTK_STOCK_IMG_SUFFIX);
	if(gc.bitmap != NULL)
	{
		gc.mask = FTK_GC_BITMAP;
		ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
		ftk_gc_reset(&gc);
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_theme_load_image(ftk_default_theme(), "close-pressed-32"FTK_STOCK_IMG_SUFFIX);
		ftk_widget_set_gc(item, FTK_WIDGET_ACTIVE, &gc);
		ftk_gc_reset(&gc);
	}
	ftk_widget_show(item, 1);
	ftk_status_item_set_clicked_listener(item, button_close_top_clicked, NULL);

	item = ftk_status_item_create(panel, 1, 32);
	ftk_widget_set_id(item, IDC_ICON_ITEM);
	gc.bitmap = ftk_theme_load_image(ftk_default_theme(), "flag-32"FTK_STOCK_IMG_SUFFIX);
	if(gc.bitmap != NULL)
	{
		gc.mask = FTK_GC_BITMAP;
		ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(item, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
		ftk_gc_reset(&gc);
	}
	ftk_widget_show(item, 1);

	item = ftk_status_item_create(panel, 2, 160);
	ftk_widget_set_id(item, IDC_TITLE_ITEM);
	ftk_widget_show(item, 1);

	ftk_wnd_manager_add_global_listener(ftk_default_wnd_manager(), on_wnd_manager_global_event, NULL);
	ftk_widget_show(panel, 1);

	return;
}

