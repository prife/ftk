/*
 * File: ftk_globals.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: some global variables.  
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

#include "ftk_globals.h"

struct _FtkGlobals
{
	FtkFont* font;
	FtkDisplay* display;
	FtkMainLoop* main_loop;
	FtkLogLevel  log_level;
	FtkWidget*   status_panel;
	FtkWndManager* wnd_manager;
	FtkBitmapFactory* bitmap_factory;
	FtkSourcesManager* sources_manager;
}g_globals;

FtkFont*          ftk_default_font(void)
{
	return g_globals.font;
}

FtkDisplay*       ftk_default_display(void)
{
	return g_globals.display;
}

FtkMainLoop*      ftk_default_main_loop(void)
{
	return g_globals.main_loop;
}

FtkLogLevel       ftk_default_log_level(void)
{
	return g_globals.log_level;
}

FtkWndManager*    ftk_default_wnd_manager(void)
{
	return g_globals.wnd_manager;
}

FtkWidget*        ftk_default_status_panel(void)
{
	return g_globals.status_panel;
}

FtkBitmapFactory* ftk_default_bitmap_factory(void)
{
	return g_globals.bitmap_factory;
}

FtkSourcesManager* ftk_default_sources_manager(void)
{
	return g_globals.sources_manager;
}

void ftk_set_font(FtkFont* font)
{
	g_globals.font = font;

	return;
}

void ftk_set_display(FtkDisplay* display)
{
	g_globals.display = display;

	return;
}

void ftk_set_main_loop(FtkMainLoop* main_loop)
{
	g_globals.main_loop = main_loop;

	return;
}

void ftk_set_log_level(FtkLogLevel level)
{
	g_globals.log_level = level;

	return;
}

void ftk_set_status_panel(FtkWidget* status_panel)
{
	g_globals.status_panel = status_panel;

	return;
}

void ftk_set_wnd_manager(FtkWndManager* wnd_manager)
{
	g_globals.wnd_manager = wnd_manager;

	return;
}

void ftk_set_bitmap_factory(FtkBitmapFactory* bitmap_factory)
{
	g_globals.bitmap_factory = bitmap_factory;

	return;
}

void ftk_set_sources_manager(FtkSourcesManager* sources_manager)
{
	g_globals.sources_manager = sources_manager;

	return;
}
