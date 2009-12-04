/*
 * File: ftk_globals.h    
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

#ifndef FTK_GLOBALS_H
#define FTK_GLOBALS_H

#include "ftk_font.h"
#include "ftk_display.h"
#include "ftk_icon_cache.h"
#include "ftk_wnd_manager.h"
#include "ftk_bitmap_factory.h"
#include "ftk_sources_manager.h"

FtkFont*          ftk_default_font(void);
FtkDisplay*       ftk_default_display(void);
FtkIconCache*     ftk_default_icon_cache(void);
FtkMainLoop*      ftk_default_main_loop(void);
FtkLogLevel       ftk_default_log_level(void);
FtkWndManager*    ftk_default_wnd_manager(void);
FtkWidget*        ftk_default_status_panel(void);
FtkBitmapFactory* ftk_default_bitmap_factory(void);
FtkSourcesManager* ftk_default_sources_manager(void);
FtkCanvas*         ftk_shared_canvas(void);

void ftk_set_font(FtkFont* font);
void ftk_set_display(FtkDisplay* display);
void ftk_set_icon_cache(FtkIconCache* icon_cache);
void ftk_set_main_loop(FtkMainLoop* main_loop);
void ftk_set_log_level(FtkLogLevel level);
void ftk_set_status_panel(FtkWidget* status_panel);
void ftk_set_wnd_manager(FtkWndManager* wnd_manager);
void ftk_set_bitmap_factory(FtkBitmapFactory* bitmap_factory);
void ftk_set_sources_manager(FtkSourcesManager* sources_manager);
void ftk_set_shared_canvas(FtkCanvas* canvas);

#endif/*FTK_GLOBALS_H*/

