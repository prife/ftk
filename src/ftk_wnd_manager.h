/*
 * File: ftk_wnd_manager.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   a simple window manager. 
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

#ifndef FTK_WND_MANAGER_H
#define FTK_WND_MANAGER_H

#include "ftk_widget.h"
#include "ftk_main_loop.h"

struct _FtkWndManager;
typedef struct _FtkWndManager FtkWndManager;

FtkWndManager* ftk_wnd_manager_create(FtkMainLoop* main_loop);
Ret  ftk_wnd_manager_grab(FtkWndManager* thiz, FtkWidget* window);
Ret  ftk_wnd_manager_add(FtkWndManager* thiz, FtkWidget* window);
Ret  ftk_wnd_manager_remove(FtkWndManager* thiz, FtkWidget* window);
Ret  ftk_wnd_manager_dispatch(FtkWndManager* thiz, FtkEvent* event);
Ret  ftk_wnd_manager_queue_event(FtkWndManager* thiz, FtkEvent* event);
Ret  ftk_wnd_manager_add_global_listener(FtkWndManager* thiz, FtkListener listener, void* ctx);
Ret  ftk_wnd_manager_remove_global_listener(FtkWndManager* thiz, FtkListener listener, void* ctx);
void ftk_wnd_manager_destroy(FtkWndManager* thiz);

#endif/*FTK_WND_MANAGER_H*/

