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
#include "ftk_button.h"

typedef struct _PrivInfo
{
	int button_down;
	char* text;
	FtkListener listener;
	void* ctx;
}PrivInfo;

#define FTK_BUTTON_LEFT_MARGIN 3
#define FTK_BUTTON_TOP_MARGIN  3

static Ret ftk_button_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV0(thiz, priv);
	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			priv->button_down = 1;
			ftk_logd("%s: FTK_EVT_MOUSE_DOWN\n", __func__);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			if(priv->button_down && priv->listener != NULL)
			{
				priv->listener(priv->ctx, thiz);
			}
			priv->button_down = 0;
			ftk_logd("%s: FTK_EVT_MOUSE_UP\n", __func__);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
			ftk_logd("%s: FTK_EVT_KEY_DOWN\n", __func__);
			if(priv->listener != NULL && event->u.key.code == FTK_KEY_ENTER)
			{
				priv->listener(priv->ctx, thiz);
			}
			break;
		}
		default:break;
	}

	return RET_OK;
}

static Ret ftk_button_on_paint(FtkWidget* thiz)
{
	FtkGc gc = {0};
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz)); 
	if(priv->text != NULL)
	{
		int fh = ftk_canvas_font_height(canvas);
		int fw = ftk_canvas_get_extent(canvas, priv->text);
		int dx = (width - fw)>>1;
		int dy = (height + 12)>>1;
	
		assert(fh < height && fw < width);
		ftk_canvas_draw_string(canvas, x + dx, y + dy, priv->text);
	}
#if 0
	if(ftk_widget_is_focused(thiz))
	{
		/*if the button is focused, use focused color*/
		gc.mask = FTK_GC_FG;
		FTK_FOCUS_COLOR(gc.fg);
		ftk_canvas_set_gc(canvas, &gc); 
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0);
	}
	else if(ftk_widget_get_gc(thiz)->bitmap != NULL)
	{
		/*if bitmap exist, clear the rect with backgroud color*/
		gc.mask = FTK_GC_FG;
		gc.fg =ftk_widget_get_gc(thiz)->bg;
		ftk_canvas_set_gc(canvas, &gc); 
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0);
	}
	else
	{
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0);
		gc.mask = FTK_GC_FG;
		gc.fg =ftk_widget_get_gc(thiz)->fg;
		gc.fg.r += 0xb0;
		gc.fg.g += 0xb0;
		gc.fg.b += 0xb0;
		ftk_canvas_set_gc(canvas, &gc); 
		ftk_canvas_draw_rect(canvas, x+1, y+1, width-2, height-2, 0);
		/*normal*/
	}
#else
	if(ftk_widget_get_gc(thiz)->bitmap != NULL)
	{
		/*if bitmap exist, clear the rect with backgroud color*/
		gc.mask = FTK_GC_FG;
		if(ftk_widget_is_focused(thiz))
		{
			FTK_FOCUS_COLOR(gc.fg);
		}
		else
		{
			gc.fg =ftk_widget_get_gc(thiz)->bg;
		}
		ftk_canvas_set_gc(canvas, &gc); 
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0);
	}
	else
	{
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0);
		gc.mask = FTK_GC_FG;
		if(ftk_widget_is_focused(thiz))
		{
			FTK_FOCUS_COLOR(gc.fg);
		}
		else
		{
			gc.fg =ftk_widget_get_gc(thiz)->fg;
			gc.fg.r += 0xb0;
			gc.fg.g += 0xb0;
			gc.fg.b += 0xb0;
		}
		ftk_canvas_set_gc(canvas, &gc); 
		ftk_canvas_draw_rect(canvas, x+1, y+1, width-2, height-2, 0);
	}
#endif
	FTK_END_PAINT();
}

static void ftk_button_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_FREE(priv->text);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_button_create(int id, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		thiz->on_event = ftk_button_on_event;
		thiz->on_paint = ftk_button_on_paint;
		thiz->destroy  = ftk_button_destroy;

		ftk_widget_init(thiz, FTK_BUTTON, id);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
	}

	return thiz;
}

Ret ftk_button_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	priv->text = strdup(text);
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_button_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}

Ret ftk_button_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->ctx = ctx;
	priv->listener = listener;

	return RET_OK;
}

