/*
 * File: ftk_widget.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   widget 
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

#include "ftk_theme.h"
#include "ftk_bitmap.h"
#include "ftk_widget.h"
#include "ftk_globals.h"
#include "ftk_canvas.h"
#include "ftk_window.h"

struct _FtkWidgetInfo
{
	int id;
	int type;
	int top;
	int left;
	int width;
	int height;
	int visible;

	int painting;
	FtkCanvas* canvas;
	FtkWidgetAttr attr;
	FtkWidgetState state;
	FtkGc gc[FTK_WIDGET_STATE_NR];
	void* user_data;
	char* text;
	size_t text_buff_length;
	FtkDestroy user_data_destroy;
};

static int  ftk_widget_is_parent_visible(FtkWidget* thiz);

void ftk_widget_init(FtkWidget* thiz, int type, int id)
{
	return_if_fail(thiz != NULL);

	thiz->ref = 1;
	thiz->priv = (FtkWidgetInfo*)FTK_ZALLOC(sizeof(FtkWidgetInfo));

	if(thiz->priv != NULL)
	{
		FtkWidgetState state = FTK_WIDGET_NORMAL;
		FtkWidgetInfo* priv =  thiz->priv;

		priv->id     = id;
		priv->type   = type;

		for(; state < FTK_WIDGET_STATE_NR; state++)
		{
			priv->gc[state].mask   = FTK_GC_BG | FTK_GC_FG | FTK_GC_FONT;
			priv->gc[state].font   = ftk_default_font();
			priv->gc[state].fg     = ftk_theme_get_fg_color(ftk_default_theme(), type, state);
			priv->gc[state].bg     = ftk_theme_get_bg_color(ftk_default_theme(), type, state);
			priv->gc[state].bitmap = ftk_theme_get_bg(ftk_default_theme(), type, state);

			if(priv->gc[state].bitmap != NULL) priv->gc[state].mask |= FTK_GC_BITMAP;
		}
	}

	return;
}

int ftk_widget_type(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->type : 0;
}

int ftk_widget_top(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->top : 0;
}

int ftk_widget_left(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->left : 0;
}

int ftk_widget_top_abs(FtkWidget* thiz)
{
	int top = 0;
	FtkWidget* iter = thiz;

	for(; iter != NULL; iter = ftk_widget_parent(iter))
	{
		top += ftk_widget_top(iter);
	}

	return top;
}

int ftk_widget_left_abs(FtkWidget* thiz)
{
	int left = 0;
	FtkWidget* iter = thiz;

	for(; iter != NULL; iter = ftk_widget_parent(iter))
	{
		left += ftk_widget_left(iter);
	}

	return left;
}

int ftk_widget_top_in_window(FtkWidget* thiz)
{
	int top = 0;
	FtkWidget* iter = thiz;

	for(; ftk_widget_parent(iter); iter = ftk_widget_parent(iter))
	{
		top += ftk_widget_top(iter);
	}

	return top;
}

int ftk_widget_left_in_window(FtkWidget* thiz)
{
	int left = 0;
	FtkWidget* iter = thiz;

	for(; ftk_widget_parent(iter); iter = ftk_widget_parent(iter))
	{
		left += ftk_widget_left(iter);
	}

	return left;
}

int ftk_widget_width(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->width : 0;
}

int ftk_widget_height(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->height : 0;
}

int ftk_widget_is_insensitive(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL && thiz->priv->state == FTK_WIDGET_INSENSITIVE;
}

int ftk_widget_is_visible(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->visible: 0;
}

int ftk_widget_is_focused(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL && thiz->priv->state == FTK_WIDGET_FOCUSED;
}

int ftk_widget_is_active(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL && thiz->priv->state == FTK_WIDGET_ACTIVE;
}

int ftk_widget_id(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->id : 0;
}

Ret ftk_widget_invalidate(FtkWidget* thiz)
{
	FtkRect rect = {0};
	if(!ftk_widget_is_visible(ftk_widget_toplevel(thiz)))
	{
		return RET_FAIL;
	}
	
	rect.y = ftk_widget_top_abs(thiz);
	rect.x = ftk_widget_left_abs(thiz);
	rect.width = ftk_widget_width(thiz);
	rect.height = ftk_widget_height(thiz);

	return ftk_window_invalidate(ftk_widget_toplevel(thiz), &rect);
}

Ret ftk_widget_update(FtkWidget* thiz)
{
	FtkEvent event = {0};
	FtkWidget* window = NULL;
	FtkWidgetInfo* priv = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	window = ftk_widget_toplevel(thiz);
	return_val_if_fail(window != NULL, RET_FAIL);

	priv =  thiz->priv;
	event.type = FTK_EVT_UPDATE;
	event.u.rect.x = ftk_widget_left_abs(thiz);
	event.u.rect.y = ftk_widget_top_abs(thiz);
	event.u.rect.width = priv->width;
	event.u.rect.height = priv->height;
		
	return ftk_widget_event(window, &event);
}

FtkCanvas* ftk_widget_canvas(FtkWidget* thiz)
{
	FtkWidget* toplevel = NULL;
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, NULL);
	
	toplevel = ftk_widget_toplevel(thiz);

	return toplevel->priv->canvas;
}

int ftk_widget_has_attr(FtkWidget* thiz, FtkWidgetAttr attr)
{
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, 0);

	return thiz->priv->attr & attr;
}

FtkWidgetState ftk_widget_state(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, 0);

	return thiz->priv->state;
}

void* ftk_widget_user_data(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, NULL);

	return thiz->priv->user_data;
}

const char* ftk_widget_get_text(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, NULL);

	return thiz->priv->text;
}

void ftk_widget_set_attr(FtkWidget* thiz, FtkWidgetAttr attr)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->attr |= attr;

	if(attr & FTK_ATTR_INSENSITIVE)
	{
		ftk_widget_set_insensitive(thiz, 1);
	}
	else if(attr & FTK_ATTR_FOCUSED)
	{
		ftk_window_set_focus(ftk_widget_toplevel(thiz), thiz);
	}

	return;
}

void ftk_widget_unset_attr(FtkWidget* thiz, FtkWidgetAttr attr)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->attr = (~attr) & thiz->priv->attr;

	return;
}

void ftk_widget_set_user_data(FtkWidget* thiz, FtkDestroy destroy, void* data)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->user_data != NULL && thiz->priv->user_data_destroy != NULL)
	{
		thiz->priv->user_data_destroy(thiz->priv->user_data);
	}
	thiz->priv->user_data = data;
	thiz->priv->user_data_destroy = destroy;

	return;
}

void ftk_widget_move(FtkWidget* thiz, int x, int y)
{
	FtkEvent event = {0};
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->left == x && thiz->priv->top  == y)
	{
		return;
	}

	thiz->priv->left = x;
	thiz->priv->top  = y;

	event.widget = thiz;
	event.type = FTK_EVT_MOVE;
	ftk_widget_event(thiz, &event);

	return;
}

void ftk_widget_resize(FtkWidget* thiz, int width, int height)
{
	FtkEvent event = {0};
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->width == width && thiz->priv->height == height)
	{
		return;
	}

	thiz->priv->width = width;
	thiz->priv->height = height;

	event.widget = thiz;
	event.type = FTK_EVT_RESIZE;
	ftk_widget_event(thiz, &event);

	return;
}

void ftk_widget_move_resize(FtkWidget* thiz, int x, int y, int width, int height)
{
	FtkEvent event = {0};
	return_if_fail(thiz != NULL && thiz->priv != NULL);
	
	if(thiz->priv->left == x && thiz->priv->top  == y
		&& thiz->priv->width == width && thiz->priv->height == height)
	{
		return;
	}

	thiz->priv->left = x;
	thiz->priv->top  = y;
	thiz->priv->width = width;
	thiz->priv->height = height;
	
	event.widget = thiz;
	event.type = FTK_EVT_MOVE_RESIZE;
	ftk_widget_event(thiz, &event);

	return;
}

void ftk_widget_set_type(FtkWidget* thiz, int type)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);
	thiz->priv->type = type;

	return;
}

void ftk_widget_set_insensitive(FtkWidget* thiz, int insensitive)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->state = insensitive ? FTK_WIDGET_INSENSITIVE : FTK_WIDGET_NORMAL;

	return;
}

static int  ftk_widget_is_parent_visible(FtkWidget* thiz)
{
	FtkWidget* parent = ftk_widget_parent(thiz);
	while(parent != NULL)
	{
		if(!ftk_widget_is_visible(parent))
		{
			return 0;
		}

		parent = ftk_widget_parent(parent);
	}

	return 1;
}

void ftk_widget_show(FtkWidget* thiz, int visible)
{
	FtkEvent event = {0};
	
	return_if_fail(thiz != NULL && thiz->priv != NULL);
	if(thiz->priv->visible == visible) return;
	
	thiz->priv->visible = visible;
	event.type = visible ? FTK_EVT_SHOW : FTK_EVT_HIDE;

	ftk_widget_event(thiz, &event);

	if(!ftk_widget_is_parent_visible(thiz))
	{
		return;
	}

	ftk_widget_invalidate(thiz);

	return;
}

void ftk_widget_show_all(FtkWidget* thiz, int visible)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);
		
	if(thiz->children != NULL)
	{
		ftk_widget_show_all(thiz->children, visible);
	}

	if(thiz->next != NULL)
	{
		ftk_widget_show_all(thiz->next, visible);
	}

	ftk_widget_show(thiz, visible);

	return;
}

void ftk_widget_set_visible(FtkWidget* thiz, int visible)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);
	if(thiz->priv->visible == visible) return;
	
	thiz->priv->visible = visible;

	return;
}

void ftk_widget_set_focused(FtkWidget* thiz, int focused)
{	
	FtkEvent event = {0};
	FtkWidgetState state = FTK_WIDGET_ACTIVE;
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->state == FTK_WIDGET_INSENSITIVE)
	{
		return;
	}

	state = focused ? FTK_WIDGET_FOCUSED : FTK_WIDGET_NORMAL;
	
	if(thiz->priv->state == state)
	{
		return;
	}

	thiz->priv->state = state;
	event.type = focused ? FTK_EVT_FOCUS_IN : FTK_EVT_FOCUS_OUT;
	event.widget = thiz;
	ftk_widget_event(thiz, &event);

	if(!ftk_widget_is_parent_visible(thiz))
	{
		return;
	}

	ftk_widget_invalidate(thiz);

	return;
}

void ftk_widget_set_active(FtkWidget* thiz, int active)
{	
	FtkWidgetState state = FTK_WIDGET_ACTIVE;
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->state == FTK_WIDGET_INSENSITIVE)
	{
		return;
	}

	state = active ? FTK_WIDGET_ACTIVE : FTK_WIDGET_FOCUSED;
	if(thiz->priv->state == state)
	{
		return;
	}
	
	thiz->priv->state = state;
	if(!ftk_widget_is_parent_visible(thiz))
	{
		return;
	}

	ftk_widget_invalidate(thiz);

	return;
}

void ftk_widget_set_id(FtkWidget* thiz, int id)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);
	
	thiz->priv->id = id;

	return;
}

void ftk_widget_set_canvas(FtkWidget* thiz, FtkCanvas* canvas)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->canvas = canvas;

	return;
}

void ftk_widget_set_parent(FtkWidget* thiz, FtkWidget* parent)
{
	FtkEvent event = {0};
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	event.u.extra = thiz;
	if(thiz->parent != NULL && parent == NULL)
	{
		event.type = FTK_EVT_REMOVE_CHILD;
		ftk_widget_event(thiz->parent, &event);
	}
	thiz->parent = parent;
	if(parent != NULL)
	{
		event.type = FTK_EVT_ADD_CHILD;
		ftk_widget_event(parent, &event);
	}

	ftk_widget_set_canvas(thiz, ftk_widget_canvas(ftk_widget_toplevel(parent)));
	
	return;
}

void ftk_widget_append_child(FtkWidget* thiz, FtkWidget* child)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->children == NULL)
	{
		thiz->children = child;
		ftk_widget_set_parent(child, thiz);
	}
	else
	{
		ftk_widget_append_sibling(thiz->children, child);
	}

	return;
}

void ftk_widget_append_sibling(FtkWidget* thiz, FtkWidget* sibling)
{
	FtkWidget* iter = thiz;
	return_if_fail(thiz != NULL && sibling != NULL);

	for(; iter->next != NULL; iter = iter->next);

	iter->next = sibling;
	sibling->prev = iter;

	ftk_widget_set_parent(sibling, ftk_widget_parent(thiz));

	return;
}

void ftk_widget_remove_child(FtkWidget* thiz, FtkWidget* child)
{
	FtkWidget* iter = thiz->children;
	return_if_fail(thiz != NULL && child != NULL);

	if(iter == child)
	{
		thiz->children = child->next;
		if(thiz->children != NULL)
		{
			thiz->children->prev = NULL;
		}

		child->prev = NULL;
		child->next = NULL;
		
		return;
	}

	for(; iter != NULL; iter = iter->next)
	{
		if(iter == child)
		{
			if(child->prev != NULL)
			{
				child->prev->next = child->next;
			}

			if(child->next != NULL)
			{
				child->next->prev = child->prev;
			}

			child->prev = NULL;
			child->next = NULL;

			return;
		}
	}
	
	return;
}

FtkWidget* ftk_widget_toplevel(FtkWidget* thiz)
{
	FtkWidget* iter = thiz;

	for(; ftk_widget_parent(iter) != NULL; iter = ftk_widget_parent(iter));

	return iter;
}

FtkWidget* ftk_widget_parent(FtkWidget* thiz)
{
	return thiz != NULL ? thiz->parent : NULL;
}

FtkWidget* ftk_widget_prev(FtkWidget* thiz)
{
	return thiz != NULL ? thiz->prev : NULL;
}

FtkWidget* ftk_widget_next(FtkWidget* thiz)
{
	return thiz != NULL ? thiz->next : NULL;
}

FtkWidget* ftk_widget_child(FtkWidget* thiz)
{
	return thiz != NULL ? thiz->children : NULL;
}

FtkWidget* ftk_widget_last_child(FtkWidget* thiz)
{
	FtkWidget* iter = NULL;
	return_val_if_fail(thiz != NULL && thiz->children != NULL, NULL);

	for(iter = thiz->children; iter->next != NULL; iter = iter->next)
	{
	}

	return iter;
}

FtkWidget* ftk_widget_lookup(FtkWidget* thiz, int id)
{
	FtkWidget* iter = thiz;
	FtkWidget* widget = NULL;
	return_val_if_fail(thiz != NULL, NULL);

	if(ftk_widget_id(thiz) == id) return iter;

	iter = ftk_widget_child(thiz);
	for(; iter != NULL; iter = ftk_widget_next(iter))
	{
		if(ftk_widget_id(iter) == id) return iter;
		if((widget = ftk_widget_lookup(iter, id)) != NULL) return widget;
	}

	return NULL;
}

void ftk_widget_paint(FtkWidget* thiz)
{
	if(!ftk_widget_is_parent_visible(thiz))
	{
		return;
	}
	
	/*If thiz is a window, we disable update first, until all sub-widgets are painted.*/
	if(ftk_widget_parent(thiz) == NULL)
	{
		ftk_window_disable_update(thiz);
	}

	ftk_widget_paint_self(thiz);

	if(ftk_widget_next(thiz) != NULL)
	{
		ftk_widget_paint(ftk_widget_next(thiz));
	}

	/*If thiz is a window, now, do real updating.*/
	if(ftk_widget_parent(thiz) == NULL)
	{
		FtkRect rect = {0};
	
		rect.x = ftk_widget_left_abs(thiz);
		rect.y = ftk_widget_top_abs(thiz);
		rect.width  = ftk_widget_width(thiz);
		rect.height = ftk_widget_height(thiz);
		ftk_window_enable_update(thiz);
		ftk_window_update(thiz, &rect);
	}

	return;
}

void    ftk_widget_set_gc(FtkWidget* thiz, FtkWidgetState state, FtkGc* gc)
{
	return_if_fail(thiz != NULL && state < FTK_WIDGET_STATE_NR && gc != NULL);

	ftk_gc_copy(thiz->priv->gc+state, gc);

	return;
}

void    ftk_widget_reset_gc(FtkWidget* thiz, FtkWidgetState state, FtkGc* gc)
{
	return_if_fail(thiz != NULL && state < FTK_WIDGET_STATE_NR && gc != NULL);

	ftk_gc_reset(thiz->priv->gc+state);
	ftk_gc_copy(thiz->priv->gc+state, gc);

	return;
}

void ftk_widget_set_text(FtkWidget* thiz, const char* text)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->text != NULL 
		&& text != NULL 
		&& thiz->priv->text_buff_length > strlen(text))
	{
		strcpy(thiz->priv->text, text);
	}
	else
	{
		FTK_FREE(thiz->priv->text);

		if(text != NULL)
		{
			thiz->priv->text = FTK_STRDUP(text);
			thiz->priv->text_buff_length = strlen(text) + 1;
		}
		else
		{
			thiz->priv->text_buff_length = 0;
		}
	}

	ftk_widget_invalidate(thiz);

	return;
}

FtkGc* ftk_widget_get_gc(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->priv != NULL, NULL);

	return thiz->priv->gc+thiz->priv->state;
}

FtkWidget* ftk_widget_find_target(FtkWidget* thiz, int x, int y)
{
	FtkWidget* target = NULL;
	int left = ftk_widget_left_abs(thiz);
	int top  = ftk_widget_top_abs(thiz);
	int w    = ftk_widget_width(thiz);
	int h    = ftk_widget_height(thiz);

	if(x < left || y < top || (x > (left + w)) || (y > (top + h)))
	{
		return NULL;
	} 

	if(thiz->children != NULL)
	{
		FtkWidget* iter = thiz->children;
		while(iter != NULL)
		{
			if((target = ftk_widget_find_target(iter, x, y)) != NULL)
			{
				return target;
			}

			iter = ftk_widget_next(iter);
		}
	}

	return thiz;
}

void ftk_widget_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		int i = 0;
		if(thiz->destroy != NULL)
		{
			thiz->destroy(thiz);
		}

		for(i = 0; i < FTK_WIDGET_STATE_NR; i++)
		{
			ftk_gc_reset(thiz->priv->gc+i);
		}

		if(thiz->priv->user_data != NULL && thiz->priv->user_data_destroy != NULL)
		{
			thiz->priv->user_data_destroy(thiz->priv->user_data);
		}

		FTK_FREE(thiz->priv->text);
		FTK_ZFREE(thiz->priv, sizeof(thiz->priv));
		FTK_ZFREE(thiz, sizeof(FtkWidget));
	}

	return;
}

void ftk_widget_ref(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->children != NULL)
		{
			ftk_widget_ref(thiz->children);
		}

		if(thiz->next != NULL)
		{
			ftk_widget_ref(thiz->next);
		}

		ftk_widget_ref_self(thiz);
	}

	return;
}

void ftk_widget_unref(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->children != NULL)
		{
			ftk_widget_unref(thiz->children);
		}

		if(thiz->next != NULL)
		{
			ftk_widget_unref(thiz->next);
		}

		ftk_widget_unref_self(thiz);
	}

	return;
}

static int ftk_widget_paint_called_by_parent(FtkWidget* thiz)
{
	FtkWidget* parent = thiz->parent;
	
	return parent != NULL ? parent->priv->painting : 0;
}

Ret ftk_widget_paint_self(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->on_paint != NULL, RET_FAIL);

	if(ftk_widget_is_visible(thiz) && ftk_widget_is_parent_visible(thiz))
	{
		FtkGc gc = {0};
		FtkBitmap* bitmap = NULL;
		FtkWidget* parent = thiz->parent;
		FtkWidgetInfo* priv =  thiz->priv;
		
		FTK_BEGIN_PAINT(x, y, width, height, canvas);
		bitmap = priv->gc[priv->state].bitmap;
		return_val_if_fail(canvas != NULL, RET_FAIL);
		return_val_if_fail(thiz->priv->width > 0 && thiz->priv->height > 0, RET_FAIL);

		priv->painting++;
		assert(parent == NULL || ftk_widget_paint_called_by_parent(thiz));

		if(!ftk_widget_has_attr(thiz, FTK_ATTR_TRANSPARENT))
		{
			gc.mask = FTK_GC_FG;
			gc.fg = ftk_widget_get_gc(thiz)->bg;
			ftk_canvas_reset_gc(canvas, &gc); 
			ftk_canvas_draw_rect(canvas, x, y, width, height, 1);
		}

		bitmap = priv->gc[priv->state].bitmap;
		if(bitmap != NULL)
		{
			if(ftk_widget_has_attr(thiz, FTK_ATTR_BG_CENTER))
			{
				ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_CENTER, x, y, width, height);
			}
			else if(ftk_widget_has_attr(thiz, FTK_ATTR_BG_TILE))
			{
				ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_TILE, x, y, width, height);
			}
			else if(ftk_widget_has_attr(thiz, FTK_ATTR_BG_FOUR_CORNER))
			{
				ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_FOUR_CORNER, x, y, width, height);
			}
			else
			{
				ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_NORMAL, x, y, width, height);
			}
		}

		if(thiz->on_paint != NULL)
		{
			thiz->on_paint(thiz);
		}
	
		if(thiz->children != NULL)
		{
			ftk_widget_paint(thiz->children);
		}
		
		priv->painting--;
	}
	
	return RET_OK;
}

void ftk_widget_ref_self(FtkWidget* thiz)
{
	return_if_fail(thiz != NULL);
	thiz->ref++;

	return;
}

void ftk_widget_unref_self(FtkWidget* thiz)
{
	return_if_fail(thiz != NULL);

	thiz->ref--;
	if(thiz->ref == 0)
	{
		ftk_widget_destroy(thiz);
	}

	return;
}

Ret ftk_widget_event(FtkWidget* thiz, FtkEvent* event)
{
	return_val_if_fail(thiz != NULL && thiz->on_event != NULL && event != NULL, RET_FAIL);

	return thiz->on_event(thiz, event);
}

