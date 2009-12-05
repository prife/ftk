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
#include "ftk_window.h"
#include "ftk_globals.h"

#define FTK_MAX_DIRTY_RECT 8

typedef struct _PrivInfo
{
	FtkCanvas*  canvas;
	FtkDisplay* display;
	FtkWidget*  focus_widget;
	FtkWidget*  grab_widget;
	int fullscreen;
	int update_disabled;
	
	FtkSource* update_idle;
	size_t dirty_rect_nr;
	FtkRect dirty_rect[FTK_MAX_DIRTY_RECT];
}PrivInfo;

static Ret ftk_window_realize(FtkWidget* thiz);

Ret ftk_window_set_focus(FtkWidget* thiz, FtkWidget* focus_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);
	
	if(priv->focus_widget == focus_widget || thiz == focus_widget)
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

Ret ftk_window_grab(FtkWidget* thiz, FtkWidget* grab_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv->grab_widget = grab_widget;
	if(priv->grab_widget != NULL)
	{
		ftk_wnd_manager_grab(ftk_default_wnd_manager(), thiz);
	}
	else
	{
		ftk_wnd_manager_ungrab(ftk_default_wnd_manager(), thiz);
	}

	return RET_OK;
}

Ret ftk_window_ungrab(FtkWidget* thiz, FtkWidget* grab_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	if(grab_widget == priv->grab_widget)
	{
		priv->grab_widget = NULL;
		ftk_wnd_manager_ungrab(ftk_default_wnd_manager(), thiz);
	}

	return RET_OK;
}

static FtkWidget* ftk_window_find_prev_focus(FtkWidget* focus_widget, int move_prev)
{
	FtkWidget* temp = NULL;
	FtkWidget* parent = NULL;
	FtkWidget* iter = move_prev ? ftk_widget_prev(focus_widget) : focus_widget;

	for(; iter != NULL; iter = ftk_widget_prev(iter))
	{
		if(ftk_widget_is_insensitive(iter) || !ftk_widget_is_visible(iter) 
			|| ftk_widget_has_attr(iter, FTK_ATTR_NO_FOCUS))
		{
			continue;
		}

		if(iter->children != NULL)
		{
			temp = ftk_window_find_prev_focus(ftk_widget_last_child(iter), 0);
			if(temp != NULL)
			{
				return temp;
			}
		}
		else
		{
			return iter;
		}
	}

	parent = ftk_widget_parent(focus_widget);
	if(parent != NULL && parent->prev != NULL)
	{
		temp = ftk_window_find_prev_focus(parent->prev, 0);
		if(temp != NULL)
		{
			return temp;
		}
	}
	
	return parent;
}

static FtkWidget* ftk_window_find_next_focus(FtkWidget* focus_widget, int move_next)
{
	FtkWidget* temp = NULL;
	FtkWidget* parent = NULL;
	FtkWidget* iter = move_next ? ftk_widget_next(focus_widget) : focus_widget;

	for(; iter != NULL; iter = ftk_widget_next(iter))
	{
		if(ftk_widget_is_insensitive(iter) || !ftk_widget_is_visible(iter)
			|| ftk_widget_has_attr(iter, FTK_ATTR_NO_FOCUS))
		{
			continue;
		}

		if(iter->children != NULL)
		{
			temp = ftk_window_find_next_focus(iter->children, 0);
			if(temp != NULL)
			{
				return temp;
			}
		}
		else
		{
			return iter;
		}
	}

	parent = ftk_widget_parent(focus_widget);
	if(parent != NULL && parent->next != NULL)
	{
		temp = ftk_window_find_next_focus(parent->next, 0);
		if(temp != NULL)
		{
			return temp;
		}
	}
	
	return focus_widget;
}

static Ret ftk_window_on_key_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;
	DECL_PRIV0(thiz, priv);
	FtkWidget* focus_widget = NULL;

	if(priv->focus_widget == NULL)
	{
		if(FTK_EVT_KEY_UP == event->type 
		&& (event->u.key.code == FTK_KEY_RIGHT || event->u.key.code == FTK_KEY_DOWN))
		{
			ftk_window_set_focus(thiz, thiz->children);
		}

		return RET_OK;
	}

	ret = ftk_widget_event(priv->focus_widget, event);
	
	if(ret == RET_REMOVE || event->type == FTK_EVT_KEY_UP)
	{
		return ret;
	}

	switch(event->u.key.code)
	{
		case FTK_KEY_LEFT:
		case FTK_KEY_UP:
		{
			focus_widget = ftk_window_find_prev_focus(priv->focus_widget, 1);
			ret = ftk_window_set_focus(thiz, focus_widget);
			break;
		}
		case FTK_KEY_DOWN:
		case FTK_KEY_RIGHT:
		case FTK_KEY_TAB:
		{
			focus_widget = ftk_window_find_next_focus(priv->focus_widget, 1);
			ret = ftk_window_set_focus(thiz, focus_widget);

			break;
		}
		default:break;
	}

	return ret;
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
		if(event->type == FTK_EVT_MOUSE_DOWN 
			&& !ftk_widget_is_insensitive(target)
			&& ftk_widget_is_visible(target)
			&& !ftk_widget_has_attr(target, FTK_ATTR_NO_FOCUS))
		{
			ftk_window_set_focus(thiz, target);
		}

		if(!ftk_widget_is_insensitive(target) && ftk_widget_is_visible(target))
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
			FtkEvent event = {0};
			event.type = FTK_EVT_SHOW;
			event.widget = thiz;
			ftk_window_realize(thiz);
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
			break;
		}
		case FTK_EVT_HIDE:
		{
			FtkEvent event = {0};
			event.type = FTK_EVT_HIDE;
			event.widget = thiz;
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
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
	DECL_PRIV0(thiz, priv);
	int w = ftk_widget_width(thiz);
	int h = ftk_widget_height(thiz);

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
		FtkGc* gc = ftk_widget_get_gc(thiz);	
		priv->canvas = ftk_canvas_create(w, h, gc->bg);
		if(gc->bitmap != NULL)
		{
			FtkBitmap* bitmap = gc->bitmap;
			if(ftk_widget_has_attr(thiz, FTK_ATTR_BG_CENTER))
			{
				ftk_canvas_set_bg_image(priv->canvas, bitmap, FTK_BG_CENTER, 0, 0, w, h);
			}
			else if(ftk_widget_has_attr(thiz, FTK_ATTR_BG_TILE))
			{
				ftk_canvas_set_bg_image(priv->canvas, bitmap, FTK_BG_TILE, 0, 0, w, h);
			}
			else
			{
				ftk_canvas_set_bg_image(priv->canvas, bitmap, FTK_BG_NORMAL, 0, 0, w, h);
			}
		}
	}
	ftk_widget_set_canvas(thiz, priv->canvas);

	return RET_OK;
}

static Ret ftk_window_on_paint(FtkWidget* thiz)
{
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

		ftk_source_unref(priv->update_idle);
		ftk_canvas_destroy(priv->canvas);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

Ret ftk_window_update(FtkWidget* thiz, FtkRect* rect)
{
	int xoffset = 0;
	int yoffset = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	if(priv->update_disabled || !ftk_widget_is_visible(thiz))
	{
		return RET_FAIL;
	}
	xoffset = ftk_widget_left(thiz) + rect->x;
	yoffset = ftk_widget_top(thiz) + rect->y;

	return ftk_display_update_and_notify(priv->display, ftk_canvas_bitmap(priv->canvas), rect, xoffset, yoffset);
}

Ret ftk_window_set_fullscreen(FtkWidget* thiz, int fullscreen)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_OK);

	if(priv->fullscreen != fullscreen)
	{
		FtkEvent event = {0};
		priv->fullscreen = fullscreen;

		event.type = FTK_EVT_RELAYOUT_WND;
		ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
	}

	return RET_OK;
}

int ftk_window_is_fullscreen(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->fullscreen;
}

static Ret ftk_window_idle_invalidate(FtkWidget* thiz)
{
	size_t i = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_REMOVE);
	
	if(priv->dirty_rect_nr == 0)
	{
		return RET_REMOVE;
	}

	ftk_window_disable_update(thiz);
	ftk_widget_paint(thiz);
	ftk_window_enable_update(thiz);

	for(i = 0; i < priv->dirty_rect_nr; i++)
	{
		ftk_window_update(thiz, priv->dirty_rect+i);
	}
	priv->dirty_rect_nr = 0;

	return RET_REMOVE;
}

Ret ftk_window_invalidate(FtkWidget* thiz, FtkRect* rect)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && rect != NULL, RET_FAIL);
	
	if(!ftk_widget_is_visible(thiz))
	{
		return RET_OK;
	}

	if((priv->dirty_rect_nr + 1) >= FTK_MAX_DIRTY_RECT)
	{
		ftk_window_idle_invalidate(thiz);
	}
	
	priv->dirty_rect[priv->dirty_rect_nr] = *rect;
	priv->dirty_rect_nr++;
		
	if(priv->dirty_rect_nr == 1)
	{
		if(priv->update_idle == NULL)
		{
			priv->update_idle = ftk_source_idle_create(ftk_window_idle_invalidate, thiz);
			ftk_source_ref(priv->update_idle);
		}
		else
		{
			ftk_source_ref(priv->update_idle);
		}
		ftk_main_loop_add_source(ftk_default_main_loop(), priv->update_idle);
	}

	return RET_OK;
}

FtkWidget* ftk_window_create(int x, int y, int width, int height)
{
	return ftk_window_create_with_type(FTK_WINDOW, x, y, width, height);
}

FtkWidget* ftk_window_create_with_type(int type, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[0] = FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		do
		{
			FtkGc gc = {0};
			DECL_PRIV0(thiz, priv);	
			if(priv == NULL)
			{
				break;
			}
			gc.mask = FTK_GC_BG | FTK_GC_FG;
			priv->display = ftk_default_display();

			ftk_widget_init(thiz, type, 0);
			ftk_widget_move(thiz, x, y);
			ftk_widget_resize(thiz, width, height);

			thiz->on_event = ftk_window_on_event;
			thiz->on_paint = ftk_window_on_paint;
			thiz->destroy  = ftk_window_destroy;

			ftk_wnd_manager_add(ftk_default_wnd_manager(), thiz);
		}while(0);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

Ret ftk_window_disable_update(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->update_disabled++;

	return RET_OK;
}

Ret ftk_window_enable_update(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->update_disabled--;

	return RET_OK;
}

Ret ftk_window_set_background_with_alpha(FtkWidget* thiz, FtkBitmap* bitmap, FtkColor bg)
{
	FtkGc gc = {0};
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	gc.mask = FTK_GC_BG;
	gc.bg = bg;
	
	if(gc.mask)
	{
		gc.mask |= FTK_GC_BITMAP;
		gc.bitmap = bitmap;
	}

	ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL,      &gc);
	ftk_widget_set_gc(thiz, FTK_WIDGET_ACTIVE,      &gc);
	ftk_widget_set_gc(thiz, FTK_WIDGET_FOCUSED,     &gc);
	ftk_widget_set_gc(thiz, FTK_WIDGET_INSENSITIVE, &gc);
	ftk_gc_reset(&gc);

	return RET_OK;
}

#ifdef FTK_WINDOW_TEST
#include "ftk_display_fb.h"

int main(int argc, char* argv[])
{
	FtkGc gc = {0};
	gc.mask = FTK_GC_FONT;
	gc.font = ftk_font_default_create("./unicode.fnt", 0, 0, 16);
	FtkDisplay* display = ftk_display_fb_create(FTK_FB_NAME);
	FtkWidget* thiz = ftk_window_create(display, 100, 0, 0, 320, 480);
	ftk_window_set_focus(thiz, NULL);
	assert(ftk_window_get_focus(thiz) == NULL);

	ftk_widget_set_text(thiz, "HelloWorld.");
	assert(strcmp(ftk_widget_get_text(thiz), "HelloWorld.") == 0);
	ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_show(thiz, 1);
	ftk_widget_paint(thiz);
	ftk_widget_unref(thiz);
	ftk_font_destroy(gc.font);
	ftk_display_destroy(display);

	return 0;
}
#endif/*FTK_WINDOW_TEST*/

