/*
 * File: ftk_animator_expand.h   
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   expand animator.
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
 * 2009-11-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_globals.h"
#include "ftk_source_timer.h"
#include "ftk_animator_expand.h"

typedef struct _PrivInfo
{
	FtkExpandType type;
	int start;
	int end;
	int step;
	int duration;
	int stop;

	int x;
	int y;
	int w;
	int h;
	int *var;
	FtkWidget* win;
	FtkCanvas* canvas;
	FtkBitmap* snap;
}PrivInfo;

static Ret  ftk_animator_expand_stop(FtkAnimator* thiz)
{
	DECL_PRIV(thiz, priv);

	priv->stop = 1;

	return RET_OK;
}

static Ret  ftk_animator_expand_init(FtkAnimator* thiz)
{
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	int w = 0;
	int h = 0;
	return_val_if_fail(priv->stop == 0, ret);

	priv->x = ftk_widget_left_abs(priv->win);
	priv->y = ftk_widget_top_abs(priv->win);
	w = ftk_widget_width(priv->win);
	h = ftk_widget_height(priv->win);

	switch(priv->type)
	{
		case FTK_ANI_TO_RIGHT:
		{
			priv->h = h;
			priv->w = priv->start;
			break;
		}
		case FTK_ANI_TO_DOWN:
		{
			priv->w = w;
			priv->h = priv->start;
			break;
		}
		case FTK_ANI_TO_EAST_SOUTH:
		default:
		{
			priv->w = priv->start;
			priv->h = priv->start * h/w;
			break;
		}
	}

	return ret;
}

static Ret  ftk_animator_expand_calc_step(FtkAnimator* thiz)
{
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv->stop == 0, ret);

	switch(priv->type)
	{
		case FTK_ANI_TO_DOWN:
		{
			ret = priv->h < priv->end ? RET_OK : RET_FAIL;
			priv->h += priv->step;
			break;
		}
		case FTK_ANI_TO_RIGHT:
		{
			ret = priv->w < priv->end ? RET_OK : RET_FAIL;
			priv->w += priv->step;
			break;
		}
		case FTK_ANI_TO_EAST_SOUTH:
		default:
		{
			int w = ftk_widget_width(priv->win);
			int h = ftk_widget_height(priv->win);
			ret = priv->w < priv->end ? RET_OK : RET_FAIL;
			priv->w += priv->step;
			priv->h += priv->step * h/w;

			break;
		}
	}

	return ret;
}

static Ret  ftk_animator_expand_step(FtkAnimator* thiz)
{
	int width    = 0;
	int height   = 0;
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	FtkBitmap* bitmap = NULL;

	if(ftk_animator_expand_calc_step(thiz) != RET_OK)
	{
		ftk_canvas_destroy(priv->canvas);
		ftk_bitmap_unref(priv->snap);
		priv->canvas = NULL;
		priv->snap   = NULL;
		ftk_window_enable_update(priv->win);
		
		return RET_REMOVE;
	}

	return_val_if_fail(priv->snap != NULL && priv->canvas != NULL, RET_REMOVE);

	width  = ftk_bitmap_width(priv->snap);
	height = ftk_bitmap_height(priv->snap);
	
	bitmap = ftk_canvas_bitmap(ftk_widget_canvas(priv->win));
	ftk_canvas_draw_bitmap(priv->canvas, bitmap, 0, 0, priv->w, priv->h, priv->x, priv->y);

	bitmap = ftk_canvas_bitmap(priv->canvas);
	rect.x = priv->x;
	rect.y = priv->y;
	rect.width  = priv->w;
	rect.height = priv->h;

	return ftk_display_update(ftk_default_display(), bitmap, &rect, priv->x, priv->y);
}

static Ret  ftk_animator_expand_start(FtkAnimator* thiz, FtkWidget* win, int sync)
{
	DECL_PRIV(thiz, priv);
	int step_duration = 0;
	FtkColor bg = {.a = 0xff};
	int width = 0;
	int height = 0;
	return_val_if_fail(win != NULL, RET_FAIL);
	return_val_if_fail(priv->start != priv->end, RET_FAIL);
	return_val_if_fail(priv->step != 0, RET_FAIL);

	step_duration = (priv->duration * priv->step) / (priv->end - priv->start);
	return_val_if_fail(step_duration > 0, RET_FAIL);

	ftk_display_snap(ftk_default_display(), &priv->snap);
	return_val_if_fail(priv->snap != NULL, RET_FAIL);
	
	width = ftk_bitmap_width(priv->snap);
	height = ftk_bitmap_height(priv->snap);
	priv->win = win;
	priv->canvas = ftk_canvas_create(width, height, bg);
	ftk_canvas_draw_bitmap(priv->canvas, priv->snap, 0, 0, width, height, 0, 0);

	ftk_window_disable_update(win);
	ftk_widget_show(win, 1);
	ftk_animator_expand_init(thiz);

	if(sync)
	{
		step_duration *= 1000;
		while(ftk_animator_expand_step(thiz) == RET_OK)
		{
			usleep(step_duration);
		}
	}
	else
	{
		FtkSource* timer = ftk_source_timer_create(step_duration, ftk_animator_expand_step, thiz);
		ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	}

	return RET_OK;
}

static Ret  ftk_animator_expand_set_param(FtkAnimator* thiz, int type, int start, int end, int step, int duration)
{
	DECL_PRIV(thiz, priv);

	priv->type     = type;
	priv->start    = start;
	priv->end      = end;
	priv->duration = duration;
	priv->step     = step;	
	if(start > end)
	{
		priv->step = step < 0 ? step : -step;
	}

	return RET_OK;
}

static void ftk_animator_expand_destroy(FtkAnimator* thiz)
{
	FTK_ZFREE(thiz, sizeof(FtkAnimator) + sizeof(PrivInfo));

	return;
}

FtkAnimator* ftk_animator_expand_create(void)
{
	FtkAnimator* thiz = (FtkAnimator*)FTK_ZALLOC(sizeof(FtkAnimator) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		thiz->stop      = ftk_animator_expand_stop;
		thiz->start     = ftk_animator_expand_start;
		thiz->set_param = ftk_animator_expand_set_param;
		thiz->destroy   = ftk_animator_expand_destroy;
	}

	return thiz;
}


