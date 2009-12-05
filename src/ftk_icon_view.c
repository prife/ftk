/*
 * File: ftk_icon_view.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   icon view
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
 * 2009-12-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_window.h"
#include "ftk_canvas.h"
#include "ftk_globals.h"
#include "ftk_icon_view.h"
#include "ftk_icon_cache.h"

typedef struct _PrivInfo
{
	size_t nr;
	size_t alloc_nr;
	FtkIconViewItem* items;
	
	int    current;
	size_t visible_nr;
	size_t visible_start;

	size_t item_width;
	size_t item_height;
	FtkListener listener;
}PrivInfo;

static Ret ftk_icon_view_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;

	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
			if(FTK_IS_ACTIVE_KEY(event->u.key.code))
			{
			}
			break;
		}
		case FTK_EVT_KEY_UP:
		{
			if(FTK_IS_ACTIVE_KEY(event->u.key.code) && ftk_widget_is_active(thiz))
			{
			}
			break;
		}
		default:break;
	}

	return ret;
}

static Ret ftk_icon_view_on_paint(FtkWidget* thiz)
{
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	ftk_canvas_reset_gc(canvas, ftk_widget_get_gc(thiz)); 
	if(ftk_widget_get_text(thiz) != NULL)
	{
		const char* text = ftk_widget_get_text(thiz);
		int fh = ftk_canvas_font_height(canvas);
		int fw = ftk_canvas_get_extent(canvas, text, -1);
		int dx = FTK_HALF(width - fw);
		int dy = FTK_HALF(height);
	
		assert(fh < height && fw < width);
		ftk_canvas_draw_string_ex(canvas, x + dx, y + dy, text, -1, 1);
	}

	FTK_END_PAINT();
}

static void ftk_icon_view_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		size_t i = 0;
		DECL_PRIV0(thiz, priv);
		for(i = 0; i < priv->nr; i++)
		{
			FtkBitmap* icon = priv->items[i].icon;
			if(icon != NULL)
			{
				ftk_bitmap_unref(icon);
			}
		}
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_icon_view_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);
	
	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		thiz->on_event = ftk_icon_view_on_event;
		thiz->on_paint = ftk_icon_view_on_paint;
		thiz->destroy  = ftk_icon_view_destroy;

		ftk_widget_init(thiz, FTK_ICON_VIEW, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);

		ftk_widget_append_child(parent, thiz);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

Ret ftk_icon_view_set_clicked_listener(FtkWidget* thiz, FtkListener listener)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->listener = listener;

	return RET_OK;
}

Ret ftk_icon_view_set_item_size(FtkWidget* thiz, size_t size)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	size = size < FTK_ICON_VIEW_ITEM_MIN ? FTK_ICON_VIEW_ITEM_MIN : size;
	size = size > FTK_ICON_VIEW_ITEM_MAX ? FTK_ICON_VIEW_ITEM_MAX : size;

	priv->item_width = size;
	priv->item_height = size;

	return RET_FAIL;
}

size_t ftk_icon_view_get_count(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->nr;
}

Ret ftk_icon_view_remove(FtkWidget* thiz, size_t index)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->nr, RET_FAIL);

	if((index + 1) < priv->nr)
	{
		for(; (index + 1) < priv->nr; index++)
		{
			priv->items[index] = priv->items[index + 1];
		}
	}

	priv->nr--;

	return RET_OK;
}

static Ret ftk_icon_view_extend(FtkWidget* thiz, size_t delta)
{
	size_t alloc_nr = 0;
	DECL_PRIV0(thiz, priv);
	FtkIconViewItem* items = NULL;

	if((priv->nr + delta) < priv->alloc_nr)
	{
		return RET_OK;
	}

	alloc_nr = priv->nr + delta + FTK_HALF(priv->nr + delta) + 5;
	items = FTK_REALLOC(priv->items, sizeof(FtkIconViewItem) + alloc_nr);
	if(items != NULL)
	{
		priv->items    = items;
		priv->alloc_nr = alloc_nr;
	}

	return (priv->nr + delta) < priv->alloc_nr ? RET_OK : RET_FAIL;
}

Ret ftk_icon_view_add(FtkWidget* thiz, const FtkIconViewItem* item)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && item != NULL, RET_FAIL);
	return_val_if_fail(ftk_icon_view_extend(thiz, 1) == RET_OK, RET_FAIL);

	priv->items[priv->nr] = *item;

	return RET_OK;
}

Ret ftk_icon_view_get(FtkWidget* thiz, size_t index, const FtkIconViewItem** item)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->nr && item != NULL, RET_FAIL);

	*item = priv->items+index;

	return RET_OK;
}

