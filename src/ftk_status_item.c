/*
 * File: ftk_status_item.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   item on the status panel. 
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
 * 2009-10-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_status_item.h"

typedef struct _PrivInfo
{
	char* text;
	FtkListener listener;
	void* listener_ctx;
	int status_item_down;
}PrivInfo;

static Ret ftk_status_item_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV0(thiz, priv);

	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			priv->status_item_down = 1;
			ftk_logd("%s: FTK_EVT_MOUSE_DOWN\n", __func__);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			if(priv->status_item_down && priv->listener != NULL)
			{
				ret = priv->listener(priv->listener_ctx, thiz);
			}
			priv->status_item_down = 0;
			ftk_logd("%s: FTK_EVT_MOUSE_UP\n", __func__);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
			ftk_logd("%s: FTK_EVT_KEY_DOWN\n", __func__);
			if(priv->listener != NULL && event->u.key.code == FTK_KEY_ENTER)
			{
				ret = priv->listener(priv->ctx, thiz);
			}
			break;
		}
		default:break;
	}

	return ret;
}

static Ret ftk_status_item_on_paint(FtkWidget* thiz)
{
	FtkGc gc = {0};
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	/*TODO*/

	FTK_END_PAINT();
}

static void ftk_status_item_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		ftk_bitmap_unref(priv->bitmap);
		FTK_FREE(priv->text);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_status_item_create(int id, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		thiz->on_event = ftk_status_item_on_event;
		thiz->on_paint = ftk_status_item_on_paint;
		thiz->destroy  = ftk_status_item_destroy;

		ftk_widget_init(thiz, FTK_STATUS_ITEM, id);
		ftk_widget_resize(thiz, width, height);
	}

	return thiz;
}

Ret ftk_status_item_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	if(priv->text != NULL)
	{
		priv->text = strdup(text);
	}
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_status_item_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}

Ret         ftk_status_item_set_bitmap(FtkWidget* thiz, FtkBitmap* bitmap)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	if(priv->bitmap != NULL)
	{
		ftk_bitmap_unref(priv->bitmap);
		priv->bitmap = NULL;
	}

	if(bitmap != NULL)
	{
		ftk_bitmap_ref(bitmap);
		priv->bitmap = bitmap;
	}

	ftk_widget_paint_self(thiz);

	return RET_OK;
}

FtkBitmap*  ftk_status_item_get_bitmap(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->bitmap;
}

Ret ftk_status_item_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->listener_ctx = ctx;
	priv->listener = listener;

	return RET_OK;
}

