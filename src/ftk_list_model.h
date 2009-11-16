/*
 * File: ftk_list_model.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   list model
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
 * 2009-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_LIST_MODEL_H
#define FTK_LIST_MODEL_H

struct _FtkListModel;
typedef struct _FtkListModel* FtkListModel;

typedef int  (*FtkListModelGetTotal)(FtkListModel* thiz);
typedef Ret  (*FtkListModelGetData)(FtkListModel* thiz, int index, void** ret);
typedef void (*FtkListModelDestroy)(FtkListModel* thiz);

struct _FtkListModel
{
	FtkListModelGetTotal get_total;
	FtkListModelGetData  get_data;
	FtkListModelDestroy  destroy;

	char priv[1];
};

#endif/*FTK_LIST_MODEL_H*/

