/*
 * File: ftk_list_model_default.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   default list model
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_list_model_default.h"

typedef struct _PrivInfo
{
	size_t nr;
	size_t alloc_nr;
	FtkListItemInfo* items;
	FtkListItemDestroy item_destroy;
}PrivInfo;

static int  ftk_list_model_default_get_total(FtkListModel* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->nr;
}

static Ret  ftk_list_model_default_get_data(FtkListModel* thiz, size_t index, void** ret)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->nr && ret != NULL, RET_FAIL);
	
	*ret = priv->items+index;
	
	return RET_OK;
}

static void ftk_list_model_default_destroy(FtkListModel* thiz)
{
	size_t i = 0;
	DECL_PRIV(thiz, priv);
	
	if(priv != NULL)
	{
		if(priv->item_destroy != NULL)
		{
			for(i = 0; i < priv->nr; i++)
			{
				priv->item_destroy(priv->items + i);
			}
		}

		FTK_FREE(priv->items);
		FTK_ZFREE(thiz, sizeof(FtkListModel) + sizeof(PrivInfo));
	}

	return;
}

static Ret ftk_list_model_default_extend(FtkListModel* thiz, size_t delta)
{
	int alloc_nr = 0;
	DECL_PRIV(thiz, priv);
	FtkListItemInfo* items = NULL;

	if(priv->items != NULL && (priv->nr + delta) < priv->alloc_nr)
	{
		return RET_OK;
	}

	alloc_nr = (priv->alloc_nr + delta) + FTK_HALF(priv->alloc_nr + delta) + 2; 
	items = FTK_REALLOC(priv->items, sizeof(FtkListItemInfo) * alloc_nr);
	if(items != NULL)
	{
		priv->items = items;
		priv->alloc_nr = alloc_nr;
	}

	return RET_OK;
}

Ret ftk_list_model_default_add(FtkListModel* thiz, void* item)
{
	DECL_PRIV(thiz, priv);
	FtkListItemInfo* info = item;
	return_val_if_fail(thiz != NULL && info != NULL, RET_FAIL);
	return_val_if_fail(ftk_list_model_default_extend(thiz, 1) == RET_OK, RET_FAIL);

	priv->items[priv->nr] = *info;
	priv->nr++;

	return RET_OK;
}

Ret ftk_list_model_default_remove(FtkListModel* thiz, size_t index)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && index < priv->nr, RET_FAIL);

	if(index < (priv->nr - 1))
	{
		size_t i = index; 
		for(; i < priv->nr; i++)
		{
			memcpy(priv->items+i, priv->items+i+1, sizeof(FtkListItemInfo));
		}
	}

	priv->nr--;

	return RET_OK;
}

FtkListModel* ftk_list_model_default_create(size_t init_nr, FtkListItemDestroy destroy)
{
	FtkListModel* thiz = FTK_ZALLOC(sizeof(FtkListModel) + sizeof(PrivInfo));
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->get_total = ftk_list_model_default_get_total;
		thiz->get_data  = ftk_list_model_default_get_data;
		thiz->add       = ftk_list_model_default_add;
		thiz->remove    = ftk_list_model_default_remove;
		thiz->destroy   = ftk_list_model_default_destroy;

		priv->alloc_nr = init_nr;
		priv->item_destroy = destroy;
	}

	return thiz;
}

