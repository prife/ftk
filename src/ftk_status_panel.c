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
#include "ftk_status_item.h"

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
	int i = 0;
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int left = 0;
	int right = 0;
	DECL_PRIV1(thiz, priv);

	if(!priv->changed)
	{
		return RET_OK;
	}
	h = ftk_widget_height(thiz);
	right = ftk_widget_width(thiz);

	for(i = 0; i < priv->first_nr; i++)
	{
		w = ftk_widget_width(priv->first[i]);
		if((x + w) >= right)
		{
			return RET_OK;
		}
		ftk_widget_move_resize(priv->first[i], x, y, w, h);
		x += w;
	}

	left = x;
	x = right;
	for(i = 0; i < priv->last_nr; i++)
	{
		w = ftk_widget_width(priv->mid[i]);
		if((x - w) < left)
		{
			return RET_OK;
		}
		x -= w;
		ftk_widget_move_resize(priv->first[i], x, y, w, h);
	}

	right = x;
	x = left;
	for(i = 0; i < priv->last_nr; i++)
	{
		w = ftk_widget_width(priv->last[i]);
		if((x + w) >= right)
		{
			return RET_OK;
		}
		ftk_widget_move_resize(priv->first[i], x, y, w, h);
		x += w;
	}
	
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

	ftk_status_panel_relayout(thiz);

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

static int ftk_status_panel_add_to(FtkWidget** array, size_t nr, int pos, FtkWidget* item)
{
	int i = 0;
	return_val_if_fail(array != NULL && nr < FTK_PANEL_MAX_ITEM && item != NULL, nr);

	for(i = 0; i < nr; i++)
	{
		if(pos > ftk_status_item_get_position(array[i]))
		{
			int j = nr - 1;
			for(; j > i; j--)
			{
				array[j] = array[j - 1];
			}
			array[i] = item;

			return RET_OK;
		}
	}

	array[nr++] = item;

	return nr;
}

static int ftk_status_panel_remove_from(FtkWidget** array, size_t nr, FtkWidget* item)
{
	int i = 0;
	return_val_if_fail(array != NULL && nr < FTK_PANEL_MAX_ITEM && item != NULL, nr);

	for(i = 0; i < nr; i++)
	{
		if(array[i] == item)
		{
			for(; i < nr; i++)
			{
				array[i] = array[i+1];
			}

			return nr - 1;
		}
	}

	return nr;
}

Ret ftk_status_panel_add(FtkWidget* thiz, int index, FtkWidget* item)
{
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(thiz != NULL && item != NULL, RET_FAIL);
	
	if(index > 0)
	{
		priv->first_nr = ftk_status_panel_add_to(priv->first, priv->first_nr, index, item);
	}
	else if(index == 0)
	{
		priv->mid_nr = ftk_status_panel_add_to(priv->mid, priv->mid_nr, index, item);
	}
	else
	{
		priv->last_nr = ftk_status_panel_add_to(priv->last, priv->last_nr, index, item);
	}

	return RET_OK;
}

Ret        ftk_status_panel_remove(FtkWidget* thiz, FtkWidget* item)
{
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(thiz != NULL && item != NULL, RET_FAIL);

	priv->first_nr = ftk_status_panel_remove_from(priv->first, priv->first_nr, item);
	priv->mid_nr = ftk_status_panel_remove_from(priv->mid, priv->mid_nr, item);
	priv->last_nr = ftk_status_panel_remove_from(priv->last, priv->last_nr, item);

	return RET_OK;
}


