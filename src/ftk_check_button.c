/*
 * File: ftk_check_button.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   check button widget 
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
 * 2009-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_canvas.h"
#include "ftk_globals.h"
#include "ftk_window.h"
#include "ftk_style.h"
#include "ftk_icon_cache.h"
#include "ftk_check_button.h"

typedef struct _PrivInfo
{
	int radio;
	int checked;
	FtkListener listener;
	void* listener_ctx;
}PrivInfo;

#define FTK_BUTTON_LEFT_MARGIN 3
#define FTK_BUTTON_TOP_MARGIN  3

static Ret ftk_check_button_on_event(FtkWidget* thiz, FtkEvent* event)
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
			if(priv->listener != NULL)
			{
				ret = priv->listener(priv->listener_ctx, thiz);
			}
			break;
		}
		case FTK_EVT_KEY_DOWN:
		{
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

static const char* check_bg_imgs[FTK_WIDGET_STATE_NR] = 
{
	[FTK_WIDGET_NORMAL] = "btn_default_normal.png",
	[FTK_WIDGET_ACTIVE] = "btn_default_pressed.png",
	[FTK_WIDGET_INSENSITIVE] = "btn_default_normal_disable.png",
	[FTK_WIDGET_FOCUSED] = "btn_default_selected.png"
};

static const char* radio_bg_imgs[FTK_WIDGET_STATE_NR] = 
{
	[FTK_WIDGET_NORMAL] = "btn_default_normal.png",
	[FTK_WIDGET_ACTIVE] = "btn_default_pressed.png",
	[FTK_WIDGET_INSENSITIVE] = "btn_default_normal_disable.png",
	[FTK_WIDGET_FOCUSED] = "btn_default_selected.png"
};

static Ret ftk_check_button_on_paint(FtkWidget* thiz)
{
	FtkBitmap* bitmap = NULL;
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	if(ftk_widget_get_gc(thiz)->bitmap == NULL)
	{
		bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), bg_imgs[ftk_widget_state(thiz)]);
		ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_FOUR_CORNER, x, y, width, height);
	}

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz)); 
	if(ftk_widget_get_text(thiz) != NULL)
	{
		const char* text = ftk_widget_get_text(thiz);
		int fh = ftk_canvas_font_height(canvas);
		int fw = ftk_canvas_get_extent(canvas, text, -1);
		int dx = (width - fw)>>1;
		int dy = (height + 12)>>1;
	
		assert(fh < height && fw < width);
		ftk_canvas_draw_string(canvas, x + dx, y + dy, text, -1);
	}

	FTK_END_PAINT();
}

static void ftk_check_button_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_check_button_create_ex(int id, int x, int y, int width, int height, int radio)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		FtkGc gc = {.mask = FTK_GC_FG | FTK_GC_BG};
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		priv->radio = radio;
		thiz->on_event = ftk_check_button_on_event;
		thiz->on_paint = ftk_check_button_on_paint;
		thiz->destroy  = ftk_check_button_destroy;

		ftk_widget_init(thiz, FTK_BUTTON, id);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);

		gc.fg = ftk_style_get_color(FTK_COLOR_BTNTEXT);
		gc.bg = ftk_style_get_color(FTK_COLOR_BTNFACE);
		ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
		
		gc.fg = ftk_style_get_color(FTK_COLOR_GRAYTEXT);
		ftk_widget_set_gc(thiz, FTK_WIDGET_INSENSITIVE, &gc);
		
		gc.fg = ftk_style_get_color(FTK_COLOR_BTNTEXT);
		gc.bg = ftk_style_get_color(FTK_COLOR_BTNHIGHLIGHT);
		ftk_widget_set_gc(thiz, FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
	}

	return thiz;
}

FtkWidget* ftk_check_button_create(int id, int x, int y, int width, int height)
{
	return ftk_check_button_create_ex(id, x, y, width, height, 0);	
}

FtkWidget* ftk_radio_button_create(int id, int x, int y, int width, int height, FtkWidget* group)
{
	FtkWidget* thiz = ftk_check_button_create_ex(id, x, y, width, height, 1);	

	
	return thiz;
}

int        ftk_check_button_get_checked(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, 0);

	return priv->checked;
}

Ret        ftk_check_button_set_checked(FtkWidget* thiz, int checked)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->checked = checked;

	return RET_OK;
}

Ret ftk_check_button_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->listener_ctx = ctx;
	priv->listener = listener;

	return RET_OK;
}

