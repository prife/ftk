/*
 * File: ftk_window.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   window
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
#include "ftk_style.h"
#include "ftk_window.h"
#include "ftk_globals.h"

typedef struct _PrivInfo
{
	char* title;
	FtkCanvas*  canvas;
	FtkDisplay* display;
	FtkWidget*  focus_widget;
	FtkWidget*  grab_widget;
	int fullscreen;
}PrivInfo;

static Ret ftk_window_realize(FtkWidget* thiz);
Ret ftk_window_set_focus(FtkWidget* thiz, FtkWidget* focus_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && thiz != focus_widget, RET_FAIL);

	if(priv->focus_widget == focus_widget)
	{
		return RET_OK;
	}

	if(priv->focus_widget != NULL)
	{
		ftk_widget_set_focused(priv->focus_widget, 0);
	}

	priv->focus_widget = focus_widget;
	
	if(priv->focus_widget != NULL)
	{
		ftk_widget_set_focused(priv->focus_widget, 1);
	}

	return RET_OK;
}

FtkWidget* ftk_window_get_focus(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->focus_widget;
}

Ret        ftk_window_grab(FtkWidget* thiz, FtkWidget* grab_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->grab_widget = grab_widget;

	return RET_OK;
}

Ret ftk_window_ungrab(FtkWidget* thiz, FtkWidget* grab_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	if(grab_widget == priv->grab_widget)
	{
		priv->grab_widget = NULL;
	}

	return RET_OK;
}

static Ret ftk_window_on_key_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;
	FtkWidget* iter = NULL;
	DECL_PRIV0(thiz, priv);

	if(priv->focus_widget == NULL)
	{
		return RET_OK;
	}

	ret = ftk_widget_event(priv->focus_widget, event);
	
	if(ret == RET_REMOVE || event->type == FTK_EVT_KEY_DOWN)
	{
		return RET_OK;
	}

	switch(event->u.key.code)
	{
		case FTK_KEY_LEFT:
		case FTK_KEY_UP:
		{
			iter = priv->focus_widget;
			while((iter = ftk_widget_prev(iter)) != NULL)
			{
				if(!ftk_widget_is_insensitive(iter) && ftk_widget_is_visible(iter))
				{
					ftk_window_set_focus(thiz, iter);
					break;
				}
			}
			break;
		}
		case FTK_KEY_DOWN:
		case FTK_KEY_RIGHT:
		case FTK_KEY_TAB:
		{
			iter = priv->focus_widget;
			while((iter = ftk_widget_next(iter)) != NULL)
			{
				if(!ftk_widget_is_insensitive(iter) && ftk_widget_is_visible(iter))
				{
					ftk_window_set_focus(thiz, iter);
					break;
				}
			}
			break;
		}
		default:break;
	}

	return RET_OK;
}

static Ret ftk_window_on_mouse_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_NO_TARGET;
	FtkWidget* target = NULL;
	DECL_PRIV0(thiz, priv);

	if(priv->grab_widget != NULL)
	{
		return ftk_widget_event(priv->grab_widget, event);
	}

	if((target = ftk_widget_find_target(thiz, event->u.mouse.x, event->u.mouse.y)) != NULL && target != thiz)
	{
		if(event->type == FTK_EVT_MOUSE_DOWN && !ftk_widget_is_insensitive(target))
		{
			ftk_window_set_focus(thiz, target);
		}

		if(!ftk_widget_is_insensitive(target))
		{
			ret = ftk_widget_event(target, event);
		}
		else
		{
			ret = RET_IGNORED;
		}
	}

	return ret;
}

static Ret ftk_window_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	switch(event->type)
	{
		case FTK_EVT_UPDATE:
		{
			ret = ftk_window_update(thiz, &event->u.rect);
			break;
		}
		case FTK_EVT_SHOW:
		{
			FtkEvent event = {.type = FTK_EVT_SHOW, .widget = thiz};
			ftk_window_realize(thiz);
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);

			break;
		}
		case FTK_EVT_HIDE:
		{
			FtkEvent event = {.type = FTK_EVT_HIDE, .widget = thiz};
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
			/*FIXME*/
			break;
		}
		case FTK_EVT_MOUSE_DOWN:
		case FTK_EVT_MOUSE_UP:
		case FTK_EVT_MOUSE_MOVE:
		{
			ret = ftk_window_on_mouse_event(thiz, event);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		case FTK_EVT_KEY_UP:
		{
			ret = ftk_window_on_key_event(thiz, event);
			break;
		}
		default:
		{
			ftk_logd("%s: type=%d\n", __func__, event->type);
			break;
		}
	}

	return ret;
}

static Ret ftk_window_realize(FtkWidget* thiz)
{
	int w = ftk_widget_width(thiz);
	int h = ftk_widget_height(thiz);
	DECL_PRIV0(thiz, priv);
	if(priv->canvas != NULL)
	{
		FtkBitmap* bitmap = ftk_canvas_bitmap(priv->canvas);
		int canvas_w = ftk_bitmap_width(bitmap);
		int canvas_h = ftk_bitmap_height(bitmap);

		if(canvas_w != w || canvas_h != h)
		{
			ftk_canvas_destroy(priv->canvas);
			priv->canvas = NULL;
		}
	}

	if(priv->canvas == NULL)
	{
		FtkColor color = ftk_style_get_color(FTK_COLOR_WINDOW);
		priv->canvas = ftk_canvas_create(w, h, color);
	}
	ftk_widget_set_canvas(thiz, priv->canvas);

	return RET_OK;
}

static Ret ftk_window_on_paint(FtkWidget* thiz)
{
	FtkRect rect = {0};
	DECL_PRIV0(thiz, priv);

	rect.width  = ftk_widget_width(thiz);
	rect.height = ftk_widget_height(thiz);

	ftk_display_update(priv->display, 
		ftk_canvas_bitmap(priv->canvas), &rect, 
		ftk_widget_left(thiz), ftk_widget_top(thiz));

	return RET_OK;
}

static void ftk_window_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		FtkEvent event = {0};
		DECL_PRIV0(thiz, priv);

		event.type = FTK_EVT_WND_DESTROY;
		event.widget = thiz;

		ftk_wnd_manager_dispatch_event(ftk_default_wnd_manager(), &event);

		FTK_FREE(priv->title);
		ftk_canvas_destroy(priv->canvas);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

Ret        ftk_window_set_title(FtkWidget* thiz, const char* title)
{
	DECL_PRIV0(thiz, priv);
	FtkEvent event = {.type = FTK_EVT_WND_CONFIG_CHANGED};
	return_val_if_fail(priv != NULL && title != NULL, RET_FAIL);

	FTK_FREE(priv->title);
	priv->title = strdup(title);

	event.widget = thiz;
	ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);

	return RET_OK;
}

const char*ftk_window_get_title(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, NULL);

	return priv->title;
}

Ret        ftk_window_update(FtkWidget* thiz, FtkRect* rect)
{
	int xoffset = 0;
	int yoffset = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	xoffset = ftk_widget_left(thiz) + rect->x;
	yoffset = ftk_widget_top(thiz) + rect->y;

	return ftk_display_update(priv->display, ftk_canvas_bitmap(priv->canvas), rect, xoffset, yoffset);
}

Ret        ftk_window_set_fullscreen(FtkWidget* thiz, int fullscreen)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_OK);

	if(priv->fullscreen != fullscreen)
	{
		priv->fullscreen = fullscreen;
		ftk_wnd_manager_update(ftk_default_wnd_manager());
	}

	return RET_OK;
}

int        ftk_window_is_fullscreen(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->fullscreen;
}

FtkWidget* ftk_window_create(int x, int y, int width, int height)
{
	return ftk_window_create_with_type(FTK_WINDOW, x, y, width, height);
}

FtkWidget* ftk_window_create_with_type(int type, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	
	if(thiz != NULL)
	{
		thiz->priv_subclass[0] = FTK_ZALLOC(sizeof(PrivInfo));
		do
		{
			FtkGc gc = {.mask = FTK_GC_BG | FTK_GC_FG};

			DECL_PRIV0(thiz, priv);	
			if(priv == NULL)
			{
				break;
			}
			priv->display = ftk_default_display();

			ftk_widget_init(thiz, type, 0);
			ftk_widget_move(thiz, x, y);
			ftk_widget_resize(thiz, width, height);

			thiz->on_event = ftk_window_on_event;
			thiz->on_paint = ftk_window_on_paint;
			thiz->destroy  = ftk_window_destroy;

			gc.bg = ftk_style_get_color(FTK_COLOR_WINDOW);
			gc.fg = ftk_style_get_color(FTK_COLOR_WINDOWTEXT);

			ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(thiz, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(thiz, FTK_WIDGET_FOCUSED, &gc);
			ftk_wnd_manager_add(ftk_default_wnd_manager(), thiz);
		}while(0);
	}

	return thiz;
}

#ifdef FTK_WINDOW_TEST
#include "ftk_display_fb.h"

int main(int argc, char* argv[])
{
	FtkGc gc = {0};
	gc.mask = FTK_GC_FONT;
	gc.font = ftk_font_create("./unicode.fnt", 16);
	FtkDisplay* display = ftk_display_fb_create(FTK_FB_NAME);
	FtkWidget* thiz = ftk_window_create(display, 100, 0, 0, 320, 480);
	ftk_window_set_focus(thiz, NULL);
	assert(ftk_window_get_focus(thiz) == NULL);

	ftk_window_set_title(thiz, "HelloWorld.");
	assert(strcmp(ftk_window_get_title(thiz), "HelloWorld.") == 0);
	ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_show(thiz, 1);
	ftk_widget_paint(thiz);
	ftk_widget_unref(thiz);
	ftk_font_destroy(gc.font);
	ftk_display_destroy(display);

	return 0;
}
#endif/*FTK_WINDOW_TEST*/

