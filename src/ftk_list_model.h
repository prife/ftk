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

#include "ftk_typedef.h"

struct _FtkListModel;
typedef struct _FtkListModel FtkListModel;

typedef int  (*FtkListModelGetTotal)(FtkListModel* thiz);
typedef Ret  (*FtkListModelGetData)(FtkListModel* thiz, int index, void** ret);
typedef void (*FtkListModelDestroy)(FtkListModel* thiz);

struct _FtkListModel
{
	FtkListModelGetTotal get_total;
	FtkListModelGetData  get_data;
	FtkListModelDestroy  destroy;

	int ref;
	char priv[1];
};

static inline int ftk_list_model_get_total(FtkListModel* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->get_total != NULL, 0);

	return thiz->get_total(thiz);
}

static inline Ret ftk_list_model_get_data(FtkListModel* thiz, int index, void** ret)
{
	return_val_if_fail(thiz != NULL && thiz->get_data != NULL, RET_FAIL);

	return thiz->get_data(thiz, index, ret);
}

static inline void ftk_list_model_destroy(FtkListModel* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
		thiz->destroy(thiz);
	}

	return;
}

static inline void ftk_list_model_ref(FtkListModel* thiz)
{
	if(thiz != NULL) thiz->ref++;

	return;
}

static inline void ftk_list_model_unref(FtkListModel* thiz)
{
	if(thiz != NULL)
	{
		thiz->ref--;
		if(thiz->ref == 0)
		{
			ftk_list_model_destroy(thiz);
		}
	}

	return;
}

#endif/*FTK_LIST_MODEL_H*/

