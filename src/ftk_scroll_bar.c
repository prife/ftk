/*
 * File: ftk_scroll_bar.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   scroll bar
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
 * 2009-11-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_scroll_bar.h"

typedef struct _PrivInfo
{
	int value;
	int max_value;
	int page_delta;
	void* listener_ctx;
	FtkListener listener;
}PrivInfo;

static Ret ftk_scroll_bar_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_scroll_bar_on_paint(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	/*TODO*/

	FTK_END_PAINT();
}

static void ftk_scroll_bar_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_scroll_bar_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz != NULL)
	{
		thiz->on_event = ftk_scroll_bar_on_event;
		thiz->on_paint = ftk_scroll_bar_on_paint;
		thiz->destroy  = ftk_scroll_bar_destroy;

		ftk_widget_init(thiz, FTK_SCROLL_BAR, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
		ftk_widget_append_child(parent, thiz);
	}

	return thiz;
}

Ret ftk_scroll_bar_set_param(FtkWidget* thiz, int max_value, int value, int page_delta)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);
	return_val_if_fail(value < max_value && page_delta < max_value, RET_FAIL);

	priv->value = value;
	priv->max_value = max_value;
	priv->page_delta = page_delta;
	ftk_widget_paint_self(thiz);	

	return RET_OK;
}

Ret ftk_scroll_bar_set_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->listener = listener;
	priv->listener_ctx = ctx;

	return RET_OK;
}

int ftk_scroll_bar_get_value(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->value;
}

int ftk_scroll_bar_get_max_value(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->max_value;
}

Ret ftk_scroll_bar_set_value(FtkWidget* thiz, int value)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);
	
	priv->value = value;
	ftk_widget_paint_self(thiz);	

	return RET_OK;
}


