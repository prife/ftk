/*
 * File: ftk_status_panel.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  ftk status panel
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
 * 2009-10-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "ftk_window.h"

#define FTK_PANEL_MAX_ITEM 16

typedef struct _PrivInfo
{
	int changed;
	int first_nr;
	int last_nr;
	int mid_nr;
	FtkWidget* first[FTK_PANEL_MAX_ITEM];	
	FtkWidget* last[FTK_PANEL_MAX_ITEM];	
	FtkWidget* mid[FTK_PANEL_MAX_ITEM];	
	FtkWidgetOnEvent parent_on_event;
	FtkWidgetOnPaint parent_on_paint;
}PrivInfo;

static Ret ftk_status_panel_relayout(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);
	if(!priv->changed)
	{
		return RET_OK;
	}

	/*TODO*/
	return RET_OK;
}

static Ret  ftk_status_panel_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV1(thiz, priv);

	return priv->parent_on_event(thiz, event);
}

static Ret  ftk_status_panel_on_paint(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);

	return priv->parent_on_paint(thiz);
}

FtkWidget* ftk_status_panel_create(int size)
{
	FtkWidget* thiz = ftk_window_create_with_type(FTK_PANEL, 0, 0, size, size);
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[1] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[1] != NULL)
	{
		DECL_PRIV1(thiz, priv);
		priv->parent_on_event = thiz->on_event;
		priv->parent_on_paint = thiz->on_paint;
		thiz->on_event = ftk_status_panel_on_event;
		thiz->on_paint = ftk_status_panel_on_paint;
	}

	return thiz;
}

static Ret ftk_status_panel_add_to(FtkWidget** array, size_t nr, int index, FtkWidget* item)
{
	/*TODO*/
	return RET_OK;
}

Ret ftk_status_panel_add(FtkWidget* thiz, int index, FtkWidget* item)
{
	Ret ret = RET_OK;
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(thiz != NULL && item != NULL, RET_FAIL);
	
	if(index > 0)
	{
		ret = ftk_status_panel_add_to(priv->first, priv->first_nr, index, item);
	}
	else if(index == 0)
	{
		ret = ftk_status_panel_add_to(priv->mid, priv->mid_nr, index, item);
	}
	else
	{
		ret = ftk_status_panel_add_to(priv->last, priv->last_nr, index, item);
	}

	return RET_OK;
}

Ret        ftk_status_panel_remove(FtkWidget* thiz, FtkWidget* item)
{
	return_val_if_fail(thiz != NULL && item != NULL, RET_FAIL);

	return RET_OK;
}


