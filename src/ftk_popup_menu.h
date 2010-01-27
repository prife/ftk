/*
 * File: ftk_popup_menu.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   popup menu
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
 * 2010-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_POPUP_MENU_H
#define FTK_POPUP_MENU_H

#include "ftk_list_model_default.h"

#define FTK_POPUP_MENU_ITEM_HEIGHT 40

FTK_BEGIN_DECLS

FtkWidget* ftk_popup_menu_create(int x, int y, int w, int h, FtkBitmap* icon, const char* title);
Ret ftk_popup_menu_init(FtkWidget* thiz, FtkListItemInfo* info, size_t nr, FtkDestroy on_item_destroy);

FTK_END_DECLS

#endif/*FTK_POPUP_MENU_H*/

