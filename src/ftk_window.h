/*
 * File: ftk_window.h    
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window 
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

#ifndef FTK_WINDOW_H
#define FTK_WINDOW_H

#include "ftk_widget.h"
#include "ftk_wnd_manager.h"

FtkWidget* ftk_window_create(int x, int y, int width, int height);
FtkWidget* ftk_window_create_with_type(int type, int x, int y, int width, int height);
Ret        ftk_window_set_focus(FtkWidget* thiz, FtkWidget* focus_widget);
FtkWidget* ftk_window_get_focus(FtkWidget* thiz);
Ret        ftk_window_grab(FtkWidget* thiz, FtkWidget* grab_widget);
Ret        ftk_window_ungrab(FtkWidget* thiz, FtkWidget* grab_widget);
Ret        ftk_window_update(FtkWidget* thiz, FtkRect* rect);
Ret        ftk_window_set_fullscreen(FtkWidget* thiz, int fullscreen);
int        ftk_window_is_fullscreen(FtkWidget* thiz);

/*for animator*/
Ret        ftk_window_disable_update(FtkWidget* thiz);
Ret        ftk_window_enable_update(FtkWidget* thiz);

#endif/*FTK_WINDOW_H*/

