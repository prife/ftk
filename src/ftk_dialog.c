/*
 * File: ftk_dialog.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
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
 * 2009-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_dialog.h"
#include "ftk_globals.h"
#include "ftk_main_loop.h"

typedef struct _PrivInfo
{
	FtkBitmap* icon;
	FtkMainLoop* main_loop;
	FtkWidgetOnEvent parent_on_event;
	FtkWidgetOnPaint parent_on_paint;
	FtkWidgetDestroy parent_destroy;
}PrivInfo;

Ret ftk_dialog_set_icon(FtkWidget* thiz, FtkBitmap* icon)
{
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	if(priv->icon != NULL)
	{
		ftk_bitmap_unref(priv->icon);
	}
	priv->icon = icon;
	if(priv->icon != NULL)
	{
		ftk_bitmap_ref(priv->icon);
	}

	return RET_OK;	
}

static Ret  ftk_dialog_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV1(thiz, priv);
	Ret ret = priv->parent_on_event(thiz, event);

	if(ret == RET_QUIT)
	{
		ftk_main_loop_quit(priv->main_loop);
	}

	switch(event->type)
	{
		case FTK_EVT_ADD_CHILD:
		{
			int x = 0;
			int y = 0;
			FtkWidget* child = event->u.extra;
			x = ftk_widget_left(child) + FTK_DIALOG_BORDER;
			y = ftk_widget_top(child) + FTK_DIALOG_TITLE_HEIGHT;

			ftk_widget_move(child, x, y);

			break;
		}
		default:break;
	}

	return RET_OK;
}

static Ret  ftk_dialog_on_paint(FtkWidget* thiz)
{
	int h = 0;
	int i = 0;
	FtkGc gc = {0};
	DECL_PRIV1(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	
	return_val_if_fail(ftk_widget_is_visible(thiz), RET_FAIL);
	
	(void)x;
	(void)y;
	gc.mask = FTK_GC_FG;
	gc.fg = ftk_widget_get_gc(thiz)->fg;
	gc.fg.r -= 0x5f;
	gc.fg.g -= 0x5f;
	gc.fg.b -= 0x5f;

	ftk_canvas_set_gc(canvas, &gc);
	for(i = 0; i < FTK_DIALOG_TITLE_HEIGHT; i++)
	{
		ftk_canvas_draw_hline(canvas, x, y+i, width);
	}

	h = height - FTK_DIALOG_TITLE_HEIGHT - FTK_DIALOG_BORDER;
	for(i = 0; i < FTK_DIALOG_BORDER; i++)
	{
		ftk_canvas_draw_hline(canvas, x, y+height-i-1, width);
		ftk_canvas_draw_vline(canvas, x+i, y+FTK_DIALOG_TITLE_HEIGHT, h);
		ftk_canvas_draw_vline(canvas, x+width-i-1, y+FTK_DIALOG_TITLE_HEIGHT, h);
	}
	
	if(priv->icon != NULL)
	{
		ftk_canvas_draw_bg_image(canvas, priv->icon, FTK_BG_CENTER, x + FTK_DIALOG_BORDER, y, 
			FTK_DIALOG_TITLE_HEIGHT, FTK_DIALOG_TITLE_HEIGHT);
	}

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz));
	if(ftk_widget_get_text(thiz) != NULL)
	{
		int xoffset = FTK_DIALOG_BORDER + (priv->icon != NULL ? FTK_DIALOG_TITLE_HEIGHT : FTK_DIALOG_BORDER);
		int yoffset = FTK_DIALOG_TITLE_HEIGHT >> 1;
		const char* text = ftk_widget_get_text(thiz);
		const char*	end = ftk_canvas_calc_str_visible_range(canvas, text, 0, -1, width - xoffset - FTK_DIALOG_BORDER);
		
		ftk_canvas_draw_string_ex(canvas, x + xoffset, y + yoffset, text, end - text, 1);
	}


	ftk_canvas_get_pixel(canvas, x, y)->a = 0;
	ftk_canvas_get_pixel(canvas, x+1, y)->a = 0;
	ftk_canvas_get_pixel(canvas, x, y+1)->a = 0;
	
	ftk_canvas_get_pixel(canvas, x+width-1, y)->a = 0;
	ftk_canvas_get_pixel(canvas, x+width-2, y)->a = 0;
	ftk_canvas_get_pixel(canvas, x+width-1, y+1)->a = 0;
	
	ftk_canvas_get_pixel(canvas, x, y+height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, x+1, y+height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, x, y+height-2)->a = 0;
	
	ftk_canvas_get_pixel(canvas, x+width-1, y+height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, x+width-2, y+height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, x+width-1, y+height-2)->a = 0;

	
	priv->parent_on_paint(thiz);

	return RET_OK;
}

static void ftk_dialog_destroy(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);
	FtkWidgetDestroy parent_destroy = priv->parent_destroy;
	parent_destroy(thiz);
	FTK_ZFREE(thiz->priv_subclass[1], sizeof(PrivInfo));

	return;
}

FtkWidget* ftk_dialog_create(int x, int y, int width, int height)
{
	FtkWidget* thiz = ftk_window_create_with_type(FTK_DIALOG, x, y, width, height);
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[1] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[1] != NULL)
	{
		DECL_PRIV1(thiz, priv);
		priv->parent_on_event = thiz->on_event;
		priv->parent_on_paint = thiz->on_paint;
		priv->parent_destroy  = thiz->destroy;
		thiz->on_event = ftk_dialog_on_event;
		thiz->on_paint = ftk_dialog_on_paint;
		thiz->destroy  = ftk_dialog_destroy;

		priv->main_loop = ftk_main_loop_create(ftk_default_sources_manager());
	}
	else
	{
		ftk_widget_destroy(thiz);
		thiz = NULL;
	}

	return thiz;
}

Ret ftk_dialog_quit(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	ftk_main_loop_quit(priv->main_loop);

	return RET_OK;
}

int ftk_dialog_run(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(ftk_widget_type(thiz) == FTK_DIALOG, RET_FAIL);

	ftk_main_loop_run(priv->main_loop);
	ftk_main_loop_destroy(priv->main_loop);
	priv->main_loop = NULL;

	return ftk_widget_id(ftk_window_get_focus(thiz));
}

