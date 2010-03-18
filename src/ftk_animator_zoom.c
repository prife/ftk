#include "ftk_globals.h"
#include "ftk_source_timer.h"
#include "ftk_animator_zoom.h"

#define ANI_ZOOM_STEPS 50
#define SINGLE_STEP_DURATION 10

typedef struct _PrivInfo
{
	int sync;
	int start;
	int end;
	int step;
	int stop;
	int duration;
	FtkZoomType type;

	int x;
	int y;
	int w;
	int h;
	FtkWidget* win;
	FtkCanvas* canvas;
	FtkBitmap* snap;

	int destroy_when_stop;
}PrivInfo;

static Ret ftk_animator_zoom_stop(FtkAnimator* thiz)
{
	DECL_PRIV(thiz, priv);
	priv->stop = 1 ;
	return RET_OK;

}

static Ret ftk_animator_zoom_init(FtkAnimator* thiz)
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
/*--will be fixed-- */
	h=h+y;
	y=0;
/*----------------*/

	priv->x = x;
	priv->y = y;
	switch(priv->type)
	{
		case FTK_ANI_ZOOM_IN:
		{
			priv->x = w/2-(w/ANI_ZOOM_STEPS)/2;
			priv->y = w/2-(h/ANI_ZOOM_STEPS)/2;
			priv->w = w/ANI_ZOOM_STEPS; 
			priv->h = h/ANI_ZOOM_STEPS;
			break;
		}
		case FTK_ANI_ZOOM_OUT:
		{
			priv->x = w/2-(w/ANI_ZOOM_STEPS*(ANI_ZOOM_STEPS-1))/2;
			priv->y = h/2-(h/ANI_ZOOM_STEPS*(ANI_ZOOM_STEPS-1))/2;
			priv->w = w/ANI_ZOOM_STEPS*(ANI_ZOOM_STEPS-1);
			priv->h = h/ANI_ZOOM_STEPS*(ANI_ZOOM_STEPS-1);
			break;
		}
		default:
			break;

	}
		
	ftk_widget_ref(priv->win);
	ftk_widget_show_all(priv->win, 1);
		
	if(priv->sync)
	{
		ftk_widget_paint(priv->win);
	}
			
	return ret;

	
}

static Ret ftk_animator_zoom_calc_step(FtkAnimator* thiz)
{
	Ret ret =  RET_FAIL;
	DECL_PRIV(thiz, priv);
	int x = ftk_widget_left(priv->win);
	int y = ftk_widget_top(priv->win);
	int w = ftk_widget_width(priv->win);
	int h = ftk_widget_height(priv->win);
/*--will be fixed-- */
	h=h+y;
	y=0;
/*----------------*/

	printf("we are in calc step: %d, %d , %d , %d\n", x, y , w, h);
	switch(priv->type)
	{
		case FTK_ANI_ZOOM_IN:
		{
			ret = priv->h < h ? RET_OK : RET_FAIL;
			priv->h += h/ANI_ZOOM_STEPS;
			priv->w += w/ANI_ZOOM_STEPS;
			priv->x -= (w/ANI_ZOOM_STEPS)/2;
			priv->y -= (h/ANI_ZOOM_STEPS)/2;
			break;

		}
		case FTK_ANI_ZOOM_OUT:
		{
			ret = priv->h > (h/ANI_ZOOM_STEPS) ? RET_OK : RET_FAIL;
			priv->h -= h/ANI_ZOOM_STEPS;
			priv->w -= w/ANI_ZOOM_STEPS;
			priv->x += (w/ANI_ZOOM_STEPS)/2;
			priv->y += (h/ANI_ZOOM_STEPS)/2;
			break;
		}
		default:
			break;
	}
	
	priv->x = priv->x >= x ? priv->x : x;
	priv->y = priv->y >= y ? priv->y : y;
	
	return ret;
}

static Ret ftk_animator_zoom_step(FtkAnimator* thiz)
{
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	FtkBitmap* bitmap = NULL;
	int x = ftk_widget_left(priv->win);
	int y = ftk_widget_top(priv->win);
	int w = ftk_widget_width(priv->win); 
	int h = ftk_widget_height(priv->win);
	printf("we are in zoom step");
/*--will be fixed-- */
	h=h+y;
	y=0;
/*----------------*/

	if(ftk_animator_zoom_calc_step(thiz) != RET_OK)
	{
#if 0 
		rect.x = x;
		rect.y = y;
		rect.width = w;
		rect.height = h; 
		bitmap = ftk_canvas_bitmap(ftk_widget_canvas(priv->canvas));
		ftk_display_update(ftk_default_display(), bitmap, &rect, x, y);
#endif 

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
		printf("we are in CALC return REMOVE\n");
		return RET_REMOVE;
	}
	
	bitmap = ftk_canvas_bitmap(ftk_widget_canvas(priv->win));
	switch(priv->type)
	{	
		case FTK_ANI_ZOOM_IN:
		{
			ftk_canvas_draw_bitmap(priv->canvas, priv->snap, 0, 0, w, h, 0, 0); 
			//fast zoom  without Fixed Point
			ftk_canvas_draw_bitmap_zoom(priv->canvas, bitmap, priv->x, priv->y, priv->w, priv->h, 0xff);
			break;

		}
		case FTK_ANI_ZOOM_OUT:
		{

			ftk_canvas_draw_bitmap(priv->canvas, bitmap, 0, 0, w, h, 0, 0);
			ftk_canvas_draw_bitmap_zoom(priv->canvas, priv->snap, priv->x, priv->y, priv->w, priv->h, 0xff);		
			break;
	
		}
		default:
			break;
	}
	rect.x = x;
	rect.y = y;
	rect.width = w;
	rect.height = h; 
	bitmap = ftk_canvas_bitmap(priv->canvas);
	
	return ftk_display_update(ftk_default_display(), bitmap, &rect, x, y);

}


static Ret ftk_animator_zoom_start(FtkAnimator* thiz, FtkWidget* win, int sync)
{
	int width = 0;
	int height = 0 ;
	int step_duration = 0;
	DECL_PRIV(thiz, priv);
	printf("OK we are in start zoom!\n");
	FtkColor bg = {0};
	FtkDisplay* display = ftk_default_display();
	return_val_if_fail(win != NULL, RET_FAIL);
	return_val_if_fail(priv->step != 0, RET_FAIL);
	return_val_if_fail(priv->duration > 0, RET_FAIL);
	return_val_if_fail(priv->start != priv->end, RET_FAIL);
	printf("OK we are in start zoom2!\n");
	bg.a = 0xff;
	priv->sync = sync;
	priv->snap = ftk_bitmap_create(ftk_display_width(display), ftk_display_height(display), bg);
	ftk_display_snap(ftk_default_display(), 0, 0, priv->snap);
	return_val_if_fail(priv->snap != NULL, RET_FAIL);
	
	priv->win = win;
	ftk_window_disable_update(win);
	width = ftk_bitmap_width(priv->snap);
	height = ftk_bitmap_height(priv->snap);
	printf("OK we are in start zoom3!\n");
	priv->canvas = ftk_canvas_create(width, height, bg);
	ftk_canvas_draw_bitmap(priv->canvas, priv->snap, 0, 0, width, height, 0, 0);
	step_duration = SINGLE_STEP_DURATION;

	ftk_animator_zoom_init(thiz);
	printf("OK we are in start zoom4!  %d \n", sync);

	if(sync)
	{
		step_duration *= 1000;
		printf("OK we are in start zoom5!\n");
		while(ftk_animator_zoom_step(thiz) == RET_OK)
		{
			usleep(step_duration);
		}
	}
	else
	{
		printf("OK we are ZOOM6\n");
		FtkSource* timer = ftk_source_timer_create(step_duration, (FtkTimer)ftk_animator_zoom_step, thiz);
		ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	}

	return RET_OK;


}

static Ret ftk_animator_zoom_set_param(FtkAnimator* thiz, int type, int start, int end , int step, int duration)
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
	else
	{
		priv->step = step > 0 ? step : -step;
	}

	return RET_OK;

}

static void ftk_animator_zoom_destroy(FtkAnimator* thiz)
{
	FTK_ZFREE(thiz, sizeof(FtkAnimator) + sizeof(PrivInfo));

	return;
}

FtkAnimator* ftk_animator_zoom_create(int destroy_when_stop)
{
	FtkAnimator* thiz = (FtkAnimator*)FTK_ZALLOC(sizeof(FtkAnimator) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->stop      = ftk_animator_zoom_stop;
		thiz->start     = ftk_animator_zoom_start;
		thiz->set_param = ftk_animator_zoom_set_param;
		thiz->destroy   = ftk_animator_zoom_destroy;

		priv->destroy_when_stop = destroy_when_stop;
	}

	return thiz;

}





