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
	int start;
	int end;
	int step;
	int stop;
	int duration;
	FtkExpandType type;

	int x;
	int y;
	int w;
	int h;
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
	int w = 0;
	int h = 0;
	int x = 0;
	int y = 0;
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv->stop == 0, ret);

	w = ftk_widget_width(priv->win);
	h = ftk_widget_height(priv->win);
	x = ftk_widget_left(priv->win);
	y = ftk_widget_top(priv->win);

	priv->x = x;
	priv->y = y;
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
		case FTK_ANI_TO_EAST_NORTH:
		{
			priv->w = priv->start;
			priv->h = priv->start * h/w;
			priv->y = y + h - priv->h;
			break;
		}
		case FTK_ANI_TO_EAST_SOUTH:
		{
			priv->w = priv->start;
			priv->h = priv->start * h/w;
			break;
		}
		default:
		case FTK_ANI_TO_UP:
		{
			priv->w = w;
			priv->y = priv->start;
			priv->h = y + h - priv->y;
			break;
		}
	}

	ftk_widget_ref(priv->win);
	ftk_widget_show(priv->win, 1);

	return ret;
}

static Ret  ftk_animator_expand_calc_step(FtkAnimator* thiz)
{
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	int x = ftk_widget_left(priv->win);
	int y = ftk_widget_top(priv->win);
	int w = ftk_widget_width(priv->win);
	int h = ftk_widget_height(priv->win);
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
		case FTK_ANI_TO_EAST_NORTH:
		{
			ret = priv->w < priv->end ? RET_OK : RET_FAIL;
			priv->w += priv->step;
			priv->h = priv->w * h/w;
			priv->y = y + h - priv->h;
			
			break;
		}
		case FTK_ANI_TO_EAST_SOUTH:
		{
			ret = priv->w < priv->end ? RET_OK : RET_FAIL;
			priv->w += priv->step;
			priv->h += priv->step * h/w;

			break;
		}
		default:
		case FTK_ANI_TO_UP:
		{
			ret = priv->y > priv->end ? RET_OK : RET_FAIL;
			priv->y += priv->step;
			priv->h  = y + h - priv->y;
			break;
		}
	}

	priv->x = priv->x >= x ? priv->x : x;
	priv->y = priv->y >= y ? priv->y : y;

	return ret;
}

static Ret  ftk_animator_expand_step(FtkAnimator* thiz)
{
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	FtkBitmap* bitmap = NULL;

	if(ftk_animator_expand_calc_step(thiz) != RET_OK)
	{
		ftk_canvas_destroy(priv->canvas);
		ftk_bitmap_unref(priv->snap);
		ftk_window_enable_update(priv->win);
		ftk_widget_show_all(priv->win, 1);
		ftk_widget_unref(priv->win);
		priv->canvas = NULL;
		priv->snap   = NULL;
		priv->win    = NULL;

		return RET_REMOVE;
	}

	bitmap = ftk_canvas_bitmap(ftk_widget_canvas(priv->win));
	ftk_canvas_draw_bitmap(priv->canvas, bitmap, 0, 0, priv->w, priv->h, priv->x, priv->y);

	rect.x = priv->x;
	rect.y = priv->y;
	rect.width  = priv->w;
	rect.height = priv->h;
	bitmap = ftk_canvas_bitmap(priv->canvas);

	return ftk_display_update(ftk_default_display(), bitmap, &rect, priv->x, priv->y);
}

static Ret  ftk_animator_expand_start(FtkAnimator* thiz, FtkWidget* win, int sync)
{
	int width = 0;
	int height = 0;
	int step_duration = 0;
	DECL_PRIV(thiz, priv);
	FtkColor bg = {0};
	FtkDisplay* display = ftk_default_display();
	return_val_if_fail(win != NULL, RET_FAIL);
	return_val_if_fail(priv->step != 0, RET_FAIL);
	return_val_if_fail(priv->duration > 0, RET_FAIL);
	return_val_if_fail(priv->start != priv->end, RET_FAIL);

	bg.a = 0xff;
	priv->snap = ftk_bitmap_create(ftk_display_width(display), ftk_display_height(display), bg);
	ftk_display_snap(ftk_default_display(), 0, 0, priv->snap);
	return_val_if_fail(priv->snap != NULL, RET_FAIL);

	priv->win = win;
	ftk_window_disable_update(win);
	width = ftk_bitmap_width(priv->snap);
	height = ftk_bitmap_height(priv->snap);
	
	priv->canvas = ftk_canvas_create(width, height, bg);
	ftk_canvas_draw_bitmap(priv->canvas, priv->snap, 0, 0, width, height, 0, 0);
	step_duration = (priv->duration * priv->step) / (priv->end - priv->start);

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
		FtkSource* timer = ftk_source_timer_create(step_duration, (FtkTimer)ftk_animator_expand_step, thiz);
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
		thiz->stop      = ftk_animator_expand_stop;
		thiz->start     = ftk_animator_expand_start;
		thiz->set_param = ftk_animator_expand_set_param;
		thiz->destroy   = ftk_animator_expand_destroy;
	}

	return thiz;
}

