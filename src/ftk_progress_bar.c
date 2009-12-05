/*
 * File: ftk_progress_bar.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   progress bar
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

#include "ftk_progress_bar.h"

typedef struct _PrivInfo
{
	int percent;
}PrivInfo;

static Ret ftk_progress_bar_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_progress_bar_on_paint(FtkWidget* thiz)
{
	int fg_width = 0;
	DECL_PRIV0(thiz, priv);
	FtkGc gc = {0};
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	return_val_if_fail(width > 4 && height > 4, RET_FAIL);

	gc.mask = FTK_GC_FG;
	gc.fg = ftk_widget_get_gc(thiz)->bg;
	ftk_canvas_set_gc(canvas, &gc);
	ftk_canvas_draw_round_rect(canvas, x, y, width, height, 1);
	
	gc.fg = ftk_widget_get_gc(thiz)->fg;
	ftk_canvas_set_gc(canvas, &gc);
	fg_width = width * priv->percent/100;

	ftk_canvas_draw_round_rect(canvas, x+1, y+1, fg_width-2, height-2, 1);
	if(fg_width < width && fg_width > 2)
	{
		ftk_canvas_draw_rect(canvas, x + fg_width - 2, y+1, 2, height-2, 1);
	}

	FTK_END_PAINT();
}

static void ftk_progress_bar_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_progress_bar_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		FtkGc gc = {0};
		gc.mask = FTK_GC_FG | FTK_GC_BG;

		thiz->on_event = ftk_progress_bar_on_event;
		thiz->on_paint = ftk_progress_bar_on_paint;
		thiz->destroy  = ftk_progress_bar_destroy;

		ftk_widget_init(thiz, FTK_PROGRESS_BAR, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);

		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
		ftk_widget_append_child(parent, thiz);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

Ret ftk_progress_bar_set_percent(FtkWidget* thiz, int percent)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(percent >=0, RET_FAIL);

	percent = percent > 100 ? 100 : percent;
	if(percent != priv->percent)
	{
		priv->percent = percent;
		ftk_widget_invalidate(thiz);
	}

	return RET_OK;
}

int ftk_progress_bar_get_percent(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, 0);

	return priv->percent;
}


