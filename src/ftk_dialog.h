/*
 * File: ftk_dialog.h    
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
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
 * 2009-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_DIALOG_H
#define FTK_DIALOG_H
#include "ftk_window.h"

FTK_INHERITE_FROM(Window)

FtkWidget* ftk_dialog_create(int x, int y, int width, int height);
int ftk_dialog_run(FtkWidget* thiz);

#endif/*FTK_DIALOG_H*/


