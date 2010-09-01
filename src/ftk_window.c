/*
 * File: ftk_window.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   window
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk.h"
#include "ftk_log.h"
#include "ftk_window.h"
#include "ftk_globals.h"
#include "ftk_main_loop.h"

#define FTK_MAX_DIRTY_RECT 16

typedef struct _PrivInfo
{
	FtkCanvas*  canvas;
	FtkDisplay* display;
	FtkWidget*  focus_widget;
	FtkWidget*  grab_widget;

	int mapped;
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

	return parent;
}

static Ret ftk_window_on_key_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;
	DECL_PRIV0(thiz, priv);
	FtkWidget* focus_widget = NULL;

	if(priv->focus_widget == NULL)
	{
		if(FTK_EVT_KEY_UP == event->type 
			&& (event->u.key.code == FTK_KEY_RIGHT 
				|| event->u.key.code == FTK_KEY_DOWN 
				|| event->u.key.code == FTK_KEY_LEFT
				|| event->u.key.code == FTK_KEY_UP))
		{
			ftk_window_set_focus(thiz, thiz->children);
		}

		return RET_OK;
	}

	ret = ftk_widget_event(priv->focus_widget, event);
	
	if(ret == RET_REMOVE)
	{
		return ret;
	}

	if(event->type == FTK_EVT_KEY_DOWN)
	{
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
	DECL_PRIV0(thiz, priv);
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
			event.widget = thiz;
			event.type = FTK_EVT_SHOW;
			ftk_window_realize(thiz);
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
			break;
		}
		case FTK_EVT_HIDE:
		{
			FtkEvent event = {0};
			event.widget = thiz;
			event.type = FTK_EVT_HIDE;
			ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);
			break;
		}
		case FTK_EVT_MAP:
		{
			priv->mapped = 1;
			PROFILE_TIME("MAP");
			ftk_logd("%s: MAP %s\n", __func__, ftk_widget_get_text(thiz));
			break;
		}
		case FTK_EVT_UNMAP:
		{
			PROFILE_TIME("UNMAP");
			ftk_logd("%s: UNMAP %s\n", __func__, ftk_widget_get_text(thiz));
			priv->mapped = 0;
			break;
		}
		case FTK_EVT_MOUSE_DOWN:
		case FTK_EVT_MOUSE_UP:
		case FTK_EVT_MOUSE_MOVE:
		case FTK_EVT_MOUSE_LONG_PRESS:
		{
			ret = ftk_window_on_mouse_event(thiz, event);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		case FTK_EVT_KEY_UP:
		case FTK_EVT_KEY_LONG_PRESS:
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

	priv->canvas = ftk_shared_canvas();
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

		if(priv->update_idle != NULL)
		{
			ftk_source_disable(priv->update_idle);
			ftk_main_loop_remove_source(ftk_default_main_loop(), priv->update_idle);
			ftk_source_unref(priv->update_idle);
		}
		FTK_ZFREE(priv, sizeof(PrivInfo));

		if(ftk_widget_has_attr(thiz, FTK_ATTR_QUIT_WHEN_CLOSE))
		{
			ftk_logd("%s: quit because window has FTK_ATTR_QUIT_WHEN_CLOSE attr\n", __func__);
			ftk_quit();
		}
	}

	return;
}

Ret ftk_window_update(FtkWidget* thiz, FtkRect* rect)
{
	int xoffset = 0;
	int yoffset = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	if(priv->update_disabled || !ftk_widget_is_visible(thiz) || !priv->mapped)
	{
		return RET_FAIL;
	}

	xoffset = rect->x;
	yoffset = rect->y;

	return ftk_display_update_and_notify(priv->display, ftk_canvas_bitmap(priv->canvas), rect, xoffset, yoffset);
}

Ret        ftk_window_paint_forcely(FtkWidget* thiz)
{
	int mapped = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	mapped = priv->mapped;
	
	priv->mapped = 1;
	ftk_widget_paint(thiz);
	priv->mapped = mapped;

	return RET_OK;
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
	FtkRect rect = {0};
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_REMOVE);
	
	if(priv->dirty_rect_nr == 0 || !priv->mapped || !ftk_widget_is_visible(thiz))
	{
		priv->dirty_rect_nr = 0;
		return RET_REMOVE;
	}

	PROFILE_TIME("begin");
	PROFILE_START();
	ftk_window_disable_update(thiz);
	ftk_widget_paint(thiz);
	ftk_window_enable_update(thiz);
	PROFILE_END("paint");

	PROFILE_START();
	if(priv->dirty_rect_nr < 3)
	{
		for(i = 0; i < priv->dirty_rect_nr; i++)
		{
			ftk_window_update(thiz, priv->dirty_rect+i);
		}
	}
	else
	{
		rect.x = ftk_widget_left_abs(thiz);
		rect.y = ftk_widget_top_abs(thiz);
		rect.width = ftk_widget_width(thiz);
		rect.height = ftk_widget_height(thiz);
		ftk_window_update(thiz, &rect);
	}
	PROFILE_END("update");
	priv->dirty_rect_nr = 0;
	PROFILE_TIME("end");

	return RET_REMOVE;
}

Ret ftk_window_invalidate(FtkWidget* thiz, FtkRect* rect)
{
	int i = 0;
	FtkRect* r = NULL;
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

	for(i = 0; i < priv->dirty_rect_nr; i++)
	{
		r = priv->dirty_rect + i;
		if(r->x == rect->x && r->x == rect->y 
			&& r->width == rect->width && r->height == rect->height)
		{
			ftk_logd("%s: remove repeated rect\n", __func__);
			return RET_OK;
		}
	}

	priv->dirty_rect[priv->dirty_rect_nr] = *rect;
	priv->dirty_rect_nr++;
		
	if(priv->dirty_rect_nr == 1)
	{
		if(priv->update_idle == NULL)
		{
			priv->update_idle = ftk_source_idle_create((FtkIdle)ftk_window_idle_invalidate, thiz);
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

FtkWidget* ftk_window_create(int type, unsigned int attr, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	PROFILE_TIME("window create begin");
	thiz->priv_subclass[0] = FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		FtkGc gc = {0};
		DECL_PRIV0(thiz, priv);	
		
		gc.mask = FTK_GC_BG | FTK_GC_FG;
		priv->display = ftk_default_display();

		thiz->on_event = ftk_window_on_event;
		thiz->on_paint = ftk_window_on_paint;
		thiz->destroy  = ftk_window_destroy;

		ftk_widget_init(thiz, type, 0, x, y, width, height, attr);

		ftk_wnd_manager_add(ftk_default_wnd_manager(), thiz);
	}
	else
	{
		FTK_FREE(thiz);
	}
	PROFILE_TIME("window create end");

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
	return_val_if_fail(priv->update_disabled > 0, RET_FAIL);

	priv->update_disabled--;

	assert(priv->update_disabled >= 0);

	return RET_OK;
}

Ret ftk_window_set_background_with_alpha(FtkWidget* thiz, FtkBitmap* bitmap, FtkColor bg)
{
	FtkGc gc = {0};
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	gc.bg = bg;
	gc.mask = FTK_GC_BG;
	
	if(bitmap != NULL)
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


