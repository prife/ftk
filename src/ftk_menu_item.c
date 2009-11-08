/*
 * File: ftk_menu_item.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   menu item. 
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
 * 2009-10-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_style.h"
#include "ftk_menu_item.h"
#include "ftk_globals.h"
#include "ftk_icon_cache.h"

typedef struct _PrivInfo
{
	char* text;
	int menu_item_down;
	FtkListener listener;
	void* listener_ctx;
}PrivInfo;

static Ret ftk_menu_item_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV0(thiz, priv);

	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			priv->menu_item_down = 1;
			ftk_widget_set_active(thiz, 1);
			ftk_logd("%s: FTK_EVT_MOUSE_DOWN\n", __func__);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			ftk_widget_set_active(thiz, 0);
			if(priv->menu_item_down && priv->listener != NULL)
			{
				ret = priv->listener(priv->listener_ctx, thiz);
			}
			priv->menu_item_down = 0;
			ftk_logd("%s: FTK_EVT_MOUSE_UP\n", __func__);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
			ftk_logd("%s: FTK_EVT_KEY_DOWN\n", __func__);
			if(priv->listener != NULL && event->u.key.code == FTK_KEY_ENTER)
			{
				ret = priv->listener(priv->listener_ctx, thiz);
			}
			break;
		}
		default:break;
	}

	return ret;
}

static const char* bg_imgs[FTK_WIDGET_STATE_NR] = 
{
	[FTK_WIDGET_ACTIVE] = "menuitem_background_pressed.png",
	[FTK_WIDGET_FOCUSED] = "menuitem_background_focus.png"
};

static Ret ftk_menu_item_on_paint(FtkWidget* thiz)
{
	FtkGc gc = {.mask =  FTK_GC_FG};
	DECL_PRIV0(thiz, priv);
	FtkBitmap* bitmap = NULL;
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	return_val_if_fail(width > 0 && height > 0, RET_FAIL);

	if(bg_imgs[ftk_widget_state(thiz)] != NULL)
	{
		bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), bg_imgs[ftk_widget_state(thiz)]);
		ftk_style_fill_background_image(canvas, FTK_BG_FOUR_CORNER, x, y, width, height, bitmap);
	}

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz));
	if(priv->text != NULL)
	{
		int fh = ftk_canvas_font_height(canvas);
		int fw = ftk_canvas_get_extent(canvas, priv->text, -1);
		int dx = (width - fw)>>1;
		int dy = (height + 12)>>1;
	
		assert(fh < height && fw < width);
		ftk_canvas_draw_string(canvas, x + dx, y + dy, priv->text, -1);
	}
	
	FTK_END_PAINT();
}

static void ftk_menu_item_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_FREE(priv->text);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_menu_item_create(int id)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		thiz->on_event = ftk_menu_item_on_event;
		thiz->on_paint = ftk_menu_item_on_paint;
		thiz->destroy  = ftk_menu_item_destroy;

		ftk_widget_init(thiz, FTK_MENU_ITEM, id);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
	}

	return thiz;
}

Ret ftk_menu_item_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	priv->text = strdup(text);
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_menu_item_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}

Ret ftk_menu_item_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->listener_ctx = ctx;
	priv->listener = listener;

	return RET_OK;
}

