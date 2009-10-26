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

	ftk_set_status_panel(ftk_status_panel_create());
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
