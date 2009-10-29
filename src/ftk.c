/*
 * File: ftk.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: ftk global init, mainloop and deinit functions.  
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
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
#include "ftk_globals.h"
#include "ftk_main_loop.h"
#include "ftk_wnd_manager_default.h"
#include "ftk_bitmap_factory.h"
#include "ftk_status_panel.h"
#include "ftk_status_item.h"

#ifdef USE_LINUX_NATIVE
#include "ftk_display_fb.h"
#include "ftk_source_input.h"
#endif

#ifdef USE_LINUX_X11
#include "ftk_display_x11.h"
#include "ftk_source_x11.h"
#endif

#ifdef HAS_JPEG
#include "ftk_image_jpeg_decoder.h"
#endif

#ifdef HAS_PNG
#include "ftk_image_png_decoder.h"
#endif

static void ftk_deinit(void);
static void ftk_init_panel(void);

#ifdef USE_LINUX_NATIVE
static Ret ftk_init_input(void)
{
	char filename[260] = {0};
	struct dirent* iter = NULL;
	DIR* dir = opendir("/dev/input");
	
	return_val_if_fail(dir != NULL, RET_FAIL);

	while((iter = readdir(dir)) != NULL)
	{
		FtkSource* source = NULL;

		if(iter->d_name[0] == '.') continue;
		if(!(iter->d_type & DT_CHR)) continue;

		snprintf(filename, sizeof(filename), "/dev/input/%s", iter->d_name);
		source = ftk_source_input_create(filename, (FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
		if(source != NULL)
		{
			ftk_sources_manager_add(ftk_default_sources_manager(), source);
		}
	}
	closedir(dir);

	return RET_OK;
}
#endif

static Ret ftk_init_bitmap_factory(void)
{
	ftk_set_bitmap_factory(ftk_bitmap_factory_create());
#ifdef HAS_JPEG
	ftk_bitmap_factory_add_decoder(ftk_default_bitmap_factory(), ftk_image_jpeg_decoder_create());	
#endif

#ifdef HAS_PNG
	ftk_bitmap_factory_add_decoder(ftk_default_bitmap_factory(), ftk_image_png_decoder_create());
#endif

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

	return;
}


Ret ftk_init(int argc, char* argv[])
{
	FtkSource* source = NULL;
	FtkDisplay* display = NULL;

	ftk_set_sources_manager(ftk_sources_manager_create(64));
	ftk_set_main_loop(ftk_main_loop_create(ftk_default_sources_manager()));
	ftk_set_wnd_manager(ftk_wnd_manager_default_create(ftk_default_main_loop()));

	ftk_init_bitmap_factory();

	ftk_set_font(ftk_font_create(FTK_FONT, 0));

#ifdef USE_LINUX_NATIVE
	(void)source;
	(void)display;
	ftk_init_input();
	ftk_set_display(ftk_display_fb_create(FTK_FB_NAME));
#endif

#ifdef USE_LINUX_X11
	display = ftk_display_x11_create(&source, (FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
	ftk_set_display(display);
	ftk_sources_manager_add(ftk_default_sources_manager(), source);
#endif

	ftk_set_status_panel(ftk_status_panel_create(FTK_STATUS_PANEL_HEIGHT));
	ftk_init_panel();

	atexit(ftk_deinit);

	return RET_OK;
}

Ret ftk_run(void)
{
	return ftk_main_loop_run(ftk_default_main_loop());
}

void ftk_quit(void)
{
	ftk_main_loop_quit(ftk_default_main_loop());

	return;
}

#define IDC_TITLE_ITEM 1000
#define IDC_ICON_ITEM  1001
#define IDC_CLOSE_ITEM 1002

static Ret on_wnd_manager_event(void* ctx, void* obj)
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
				ftk_status_item_set_text(title_widget, ftk_window_get_title(top_window));
			}
			else
			{
				ftk_status_item_set_text(title_widget, NULL);
			}
			ftk_widget_set_user_data(title_widget, NULL, top_window);
			ftk_logd("top_window changed: %s\n", ftk_window_get_title(top_window));
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
				ftk_status_item_set_text(title_widget, ftk_window_get_title(top_window));
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

	if(top_window != NULL)
	{
		ftk_logd("%s: close window %s\n", __func__, ftk_window_get_title(top_window));
		ftk_widget_unref(top_window);
		ftk_widget_set_user_data(title_widget, NULL, NULL);
	}

	return RET_OK;
}

static void ftk_init_panel(void)
{
	FtkGc gc = {0};
	FtkWidget* item = NULL;	
	FtkWidget* panel = ftk_default_status_panel();

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/status-bg.png");
	ftk_widget_set_gc(panel, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(panel, FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/close-32.png");
	item = ftk_status_item_create(IDC_CLOSE_ITEM, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
	ftk_status_panel_add(panel, -1, item);
	ftk_widget_show(item, 1);
	ftk_status_item_set_clicked_listener(item, button_close_top_clicked, NULL);
	ftk_gc_reset(&gc);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/flag-32.png");
	item = ftk_status_item_create(IDC_ICON_ITEM, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
	ftk_status_panel_add(panel, 1, item);
	ftk_widget_show(item, 1);
	ftk_gc_reset(&gc);

	item = ftk_status_item_create(IDC_TITLE_ITEM, 160, 32);
	ftk_status_panel_add(panel, 2, item);
	ftk_widget_show(item, 1);

	ftk_wnd_manager_add_global_listener(ftk_default_wnd_manager(), on_wnd_manager_event, NULL);
	ftk_widget_show(panel, 1);

	return;
}
