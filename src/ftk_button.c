/*
 * File: ftk_button.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   button. 
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_theme.h"
#include "ftk_window.h"
#include "ftk_canvas.h"
#include "ftk_button.h"
#include "ftk_globals.h"
#include "ftk_icon_cache.h"

typedef struct _PrivInfo
{
	FtkListener listener;
	void* listener_ctx;
}PrivInfo;

static Ret ftk_button_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV0(thiz, priv);
	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			ftk_widget_set_active(thiz, 1);
			ftk_window_grab(ftk_widget_toplevel(thiz), thiz);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			ftk_widget_set_active(thiz, 0);
			ftk_window_ungrab(ftk_widget_toplevel(thiz), thiz);
			ret = FTK_CALL_LISTENER(priv->listener, priv->listener_ctx, thiz);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
			if(FTK_IS_ACTIVE_KEY(event->u.key.code))
			{
				ftk_widget_set_active(thiz, 1);
			}
			break;
		}
		case FTK_EVT_KEY_UP:
		{
			if(FTK_IS_ACTIVE_KEY(event->u.key.code) && ftk_widget_is_active(thiz))
			{
				ftk_widget_set_active(thiz, 0);
				ret = FTK_CALL_LISTENER(priv->listener, priv->listener_ctx, thiz);
			}
			break;
		}
		default:break;
	}

	return ret;
}

static Ret ftk_button_on_paint(FtkWidget* thiz)
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

static void ftk_button_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_button_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);
	
	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		FtkGc gc ={0};

		thiz->on_event = ftk_button_on_event;
		thiz->on_paint = ftk_button_on_paint;
		thiz->destroy  = ftk_button_destroy;

		ftk_widget_init(thiz, FTK_BUTTON, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT|FTK_ATTR_BG_FOUR_CORNER);
		ftk_widget_append_child(parent, thiz);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

Ret ftk_button_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->listener_ctx = ctx;
	priv->listener = listener;

	return RET_OK;
}

