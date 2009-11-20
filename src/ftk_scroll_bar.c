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

#include "ftk_globals.h"
#include "ftk_scroll_bar.h"

typedef struct _PrivInfo
{
	int value;
	int max_value;
	int page_delta;
	int mouse_pressed;
	int pressed_x;
	int pressed_y;
	int vertical;
	void* listener_ctx;
	FtkBitmap* bitmap;
	FtkListener listener;
}PrivInfo;

static Ret ftk_scroll_bar_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV0(thiz, priv);
	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			priv->mouse_pressed = 1;		
			priv->pressed_x = event->u.mouse.x;
			priv->pressed_y = event->u.mouse.y;
			ftk_window_grab(ftk_widget_toplevel(thiz), thiz);
			break;
		}
		case FTK_EVT_MOUSE_MOVE:
		{
			int value = 0;
			if(priv->mouse_pressed)
			{
			}
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			priv->mouse_pressed = 0;
			ftk_window_ungrab(ftk_widget_toplevel(thiz), thiz);
			break;
		}
		default:break;
	}

	return RET_OK;
}

static Ret ftk_scroll_bar_on_paint(FtkWidget* thiz)
{
	int dx = 0;
	int dy = 0;
	int bitmap_width = 0;
	int bitmap_height = 0;
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	return_val_if_fail(priv->bitmap != NULL, RET_FAIL);

	bitmap_width = ftk_bitmap_width(priv->bitmap);
	bitmap_height = ftk_bitmap_height(priv->bitmap);
	if(priv->vertical)
	{
		dy = height * priv->value / priv->max_value;
		dy = (dy + bitmap_height) < height ? dy : height - bitmap_height;
	}
	else
	{
		dx = width * priv->value / priv->max_value;
		dx = (dx + bitmap_width) < width ? dx : width - bitmap_width;
	}

	ftk_canvas_draw_bitmap(canvas, priv->bitmap, 0, 0, bitmap_width, bitmap_height, x + dx, y + dy);

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
		DECL_PRIV0(thiz, priv);
		thiz->on_event = ftk_scroll_bar_on_event;
		thiz->on_paint = ftk_scroll_bar_on_paint;
		thiz->destroy  = ftk_scroll_bar_destroy;
		if(width < height)
		{
			/*vertical*/
			priv->vertical = 1;
			priv->bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), 
				"scrollbar_handle_vertical"FTK_STOCK_IMG_SUFFIX);
			width = ftk_bitmap_width(priv->bitmap);
			assert(width < height);
		}
		else
		{
			priv->vertical = 0;
			priv->bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), 
				"scrollbar_handle_horizontal"FTK_STOCK_IMG_SUFFIX);

			height = ftk_bitmap_height(priv->bitmap);	
			assert(width > height);
		}

		ftk_widget_init(thiz, FTK_SCROLL_BAR, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
		ftk_widget_append_child(parent, thiz);
	}

	return thiz;
}

Ret ftk_scroll_bar_set_param(FtkWidget* thiz, int value, int max_value, int page_delta)
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


