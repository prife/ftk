/*
 * File: ftk_dialog.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
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
 * 2009-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_dialog.h"
#include "ftk_globals.h"
#include "ftk_main_loop.h"

typedef struct _PrivInfo
{
	FtkMainLoop* main_loop;
	FtkWidgetOnEvent parent_on_event;
	FtkWidgetOnPaint parent_on_paint;
	FtkWidgetDestroy parent_destroy;
}PrivInfo;

static Ret  ftk_dialog_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV1(thiz, priv);
	Ret ret = priv->parent_on_event(thiz, event);

	if(ret == RET_QUIT)
	{
		ftk_main_loop_quit(priv->main_loop);
	}

	return RET_OK;
}

static Ret  ftk_dialog_on_paint(FtkWidget* thiz)
{
	int i = 0;
	FtkGc gc = {0};
	DECL_PRIV1(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	
	return_val_if_fail(ftk_widget_is_visible(thiz), RET_FAIL);

	(void)x;
	(void)y;
	gc.mask = FTK_GC_FG;
	gc.fg = ftk_widget_get_gc(thiz)->fg;
	gc.fg.r -= 0x2f;
	gc.fg.g -= 0x2f;
	gc.fg.b -= 0x2f;
	ftk_canvas_set_gc(canvas, &gc);
	for(i = 0; i < 10; i++)
	{
		ftk_canvas_draw_hline(canvas, 0, i, width);
	}
	for(i = 0; i < 10; i++)
	{
		ftk_canvas_draw_hline(canvas, 0, height-i, width);
	}
	ftk_canvas_draw_vline(canvas, 0, 10, height - 20);
	ftk_canvas_draw_vline(canvas, 1, 10, height - 20);
	ftk_canvas_draw_vline(canvas, width-1, 10, height - 20);
	ftk_canvas_draw_vline(canvas, width-2, 10, height - 20);
	
	ftk_canvas_get_pixel(canvas, 0, 0)->a = 0;
	ftk_canvas_get_pixel(canvas, 1, 0)->a = 0;
	ftk_canvas_get_pixel(canvas, 0, 1)->a = 0;
	
	ftk_canvas_get_pixel(canvas, width-1, 0)->a = 0;
	ftk_canvas_get_pixel(canvas, width-2, 0)->a = 0;
	ftk_canvas_get_pixel(canvas, width-1, 1)->a = 0;
	
	ftk_canvas_get_pixel(canvas, 0, height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, 1, height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, 0, height-2)->a = 0;
	
	ftk_canvas_get_pixel(canvas, width-1, height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, width-2, height-1)->a = 0;
	ftk_canvas_get_pixel(canvas, width-1, height-2)->a = 0;

	priv->parent_on_paint(thiz);

	return RET_OK;
}

static void ftk_dialog_destroy(FtkWidget* thiz)
{
	DECL_PRIV1(thiz, priv);
	FtkWidgetDestroy parent_destroy = priv->parent_destroy;
	FTK_ZFREE(thiz->priv_subclass[1], sizeof(PrivInfo));
	parent_destroy(thiz);

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

