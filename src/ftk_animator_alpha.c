/*
 * File: ftk_animator_alpha.h   
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   alpha animator.
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
 * 2010-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_globals.h"
#include "ftk_source_timer.h"
#include "ftk_animator_alpha.h"

typedef struct _PrivInfo
{
	int  start;
	int  end;
	char step;
	int  stop;
	int duration;

	FtkWidget* win;
	FtkBitmap* snap;
	FtkCanvas* canvas;

	int destroy_when_stop;
}PrivInfo;

static Ret  ftk_animator_alpha_stop(FtkAnimator* thiz)
{
	DECL_PRIV(thiz, priv);

	priv->stop = 1;

	return RET_OK;
}

static Ret  ftk_animator_alpha_init(FtkAnimator* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv->stop == 0, RET_FAIL);

	ftk_widget_ref(priv->win);
	ftk_widget_show(priv->win, 1);

	return RET_OK;
}

static Ret  ftk_animator_alpha_calc_step(FtkAnimator* thiz)
{
	DECL_PRIV(thiz, priv);
	
	if(priv->step > 0)
	{
		priv->start += priv->step;
		priv->start = priv->start > priv->end ? priv->end : priv->start;
	}
	else
	{
		priv->start += priv->step;
		priv->start = priv->start < priv->end ? priv->end : priv->start;
	}

	return (priv->start != priv->end) ? RET_OK : RET_REMOVE;
}

static Ret  ftk_animator_alpha_step(FtkAnimator* thiz)
{
	FtkGc gc = {0};
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	FtkBitmap* bitmap = NULL;
	
	int x = ftk_widget_left(priv->win);
	int y = ftk_widget_top(priv->win);
	int w = ftk_bitmap_width(priv->snap);
	int h = ftk_bitmap_height(priv->snap);

	Ret ret = ftk_animator_alpha_calc_step(thiz);

	ftk_canvas_reset_gc(priv->canvas, &gc);
	ftk_canvas_draw_bitmap(priv->canvas, priv->snap, 0, 0, w, h, 0, 0);

	w = ftk_widget_width(priv->win);
	h = ftk_widget_height(priv->win);
	bitmap = ftk_canvas_bitmap(ftk_widget_canvas(priv->win));

	gc.mask = FTK_GC_ALPHA;
	gc.alpha = priv->start & 0xFF;

	ftk_canvas_set_gc(priv->canvas, &gc);
	ftk_canvas_draw_bitmap(priv->canvas, bitmap, 0, 0, w, h, x,y);
	bitmap = ftk_canvas_bitmap(priv->canvas);

	rect.x = x;
	rect.y = y;
	rect.width = w;
	rect.height = h;

	ftk_logd("%s: x=%d y=%d\n", __func__, x, y);
	ftk_display_update(ftk_default_display(), bitmap, &rect, x, y);

	if(ret != RET_OK)
	{
		ftk_canvas_destroy(priv->canvas);
		ftk_bitmap_unref(priv->snap);
		ftk_window_enable_update(priv->win);
		ftk_widget_show_all(priv->win, 1);
		ftk_widget_unref(priv->win);
		priv->canvas = NULL;
		priv->snap   = NULL;
		priv->win    = NULL;
		
		if(priv->destroy_when_stop)
		{
			ftk_animator_destroy(thiz);
		}

		return RET_REMOVE;
	}

	return ret;
}

static Ret  ftk_animator_alpha_start(FtkAnimator* thiz, FtkWidget* win, int sync)
{
	int width = 0;
	int height = 0;
	FtkColor bg = {0};
	int step_duration = 0;
	DECL_PRIV(thiz, priv);
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
	
	step_duration = (priv->duration * priv->step) / (priv->end - priv->start);

	ftk_animator_alpha_init(thiz);

	if(sync)
	{
		step_duration *= 1000;
		while(ftk_animator_alpha_step(thiz) == RET_OK)
		{
			usleep(step_duration);
		}
	}
	else
	{
		FtkSource* timer = ftk_source_timer_create(step_duration, (FtkTimer)ftk_animator_alpha_step, thiz);
		ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	}

	return RET_OK;
}

static Ret  ftk_animator_alpha_set_param(FtkAnimator* thiz, int type, int start, int end, int step, int duration)
{
	DECL_PRIV(thiz, priv);
	
	priv->start    = start & 0xFF;
	priv->end      = end   & 0xFF;
	priv->duration = duration;
	priv->step     = step & 0xFF;	

	if(start > end)
	{
		priv->step = step < 0 ? step : -step;
	}
	else
	{
		priv->step = step > 0 ? step : -step;
	}

	return RET_OK;
}

static void ftk_animator_alpha_destroy(FtkAnimator* thiz)
{
	FTK_ZFREE(thiz, sizeof(FtkAnimator) + sizeof(PrivInfo));

	return;
}

FtkAnimator* ftk_animator_alpha_create(int destroy_when_stop)
{
	FtkAnimator* thiz = (FtkAnimator*)FTK_ZALLOC(sizeof(FtkAnimator) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->stop      = ftk_animator_alpha_stop;
		thiz->start     = ftk_animator_alpha_start;
		thiz->set_param = ftk_animator_alpha_set_param;
		thiz->destroy   = ftk_animator_alpha_destroy;

		priv->destroy_when_stop = destroy_when_stop;
	}

	return thiz;
}

