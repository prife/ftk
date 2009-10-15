/*
 * File: ftk_widget.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   widget 
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

#include "ftk_bitmap.h"
#include "ftk_widget.h"
#include "ftk_globals.h"

struct _FtkWidgetInfo
{
	int id;
	int type;
	int top;
	int left;
	int width;
	int height;
	int visible;

	FtkCanvas* canvas;
	FtkWidgetState state;
	FtkGc gc[FTK_WIDGET_STATE_NR];
};

static int  ftk_widget_is_parent_visible(FtkWidget* thiz);

void ftk_widget_init(FtkWidget* thiz, int type, int id)
{
	return_if_fail(thiz != NULL);

	thiz->ref = 1;
	thiz->priv = (FtkWidgetInfo*)FTK_ZALLOC(sizeof(FtkWidgetInfo));

	if(thiz->priv != NULL)
	{
		FtkWidgetInfo* priv =  thiz->priv;

		priv->id     = id;
		priv->type   = type;
		priv->gc[FTK_WIDGET_NORMAL].mask = FTK_GC_BG | FTK_GC_FG | FTK_GC_FONT;
		priv->gc[FTK_WIDGET_NORMAL].fg.a = 0xff;
		priv->gc[FTK_WIDGET_NORMAL].fg.r = 0x00;
		priv->gc[FTK_WIDGET_NORMAL].fg.g = 0x00;
		priv->gc[FTK_WIDGET_NORMAL].fg.b = 0x00;
		priv->gc[FTK_WIDGET_NORMAL].font = ftk_default_font();
		priv->gc[FTK_WIDGET_NORMAL].bg.a = 0xff;
		priv->gc[FTK_WIDGET_NORMAL].bg.r = 0xe0;
		priv->gc[FTK_WIDGET_NORMAL].bg.g = 0xd0;
		priv->gc[FTK_WIDGET_NORMAL].bg.b = 0xe0;
		
		priv->gc[FTK_WIDGET_FOCUSED].mask = FTK_GC_BG | FTK_GC_FG | FTK_GC_FONT;
		priv->gc[FTK_WIDGET_FOCUSED].fg.a = 0xff;
		priv->gc[FTK_WIDGET_FOCUSED].fg.r = 0x00;
		priv->gc[FTK_WIDGET_FOCUSED].fg.g = 0x00;
		priv->gc[FTK_WIDGET_FOCUSED].fg.b = 0x00;
		priv->gc[FTK_WIDGET_FOCUSED].font = ftk_default_font();
		priv->gc[FTK_WIDGET_FOCUSED].bg.a = 0xff;
		priv->gc[FTK_WIDGET_FOCUSED].bg.r = 0xe0;
		priv->gc[FTK_WIDGET_FOCUSED].bg.g = 0xd0;
		priv->gc[FTK_WIDGET_FOCUSED].bg.b = 0xe0;
		
		priv->gc[FTK_WIDGET_INSENSITIVE].mask = FTK_GC_BG | FTK_GC_FG | FTK_GC_FONT;
		priv->gc[FTK_WIDGET_INSENSITIVE].fg.a = 0xff;
		priv->gc[FTK_WIDGET_INSENSITIVE].fg.r = 0x00;
		priv->gc[FTK_WIDGET_INSENSITIVE].fg.g = 0x00;
		priv->gc[FTK_WIDGET_INSENSITIVE].fg.b = 0x00;
		priv->gc[FTK_WIDGET_INSENSITIVE].font = ftk_default_font();
		priv->gc[FTK_WIDGET_INSENSITIVE].bg.a = 0xff;
		priv->gc[FTK_WIDGET_INSENSITIVE].bg.r = 0xe0;
		priv->gc[FTK_WIDGET_INSENSITIVE].bg.g = 0xd0;
		priv->gc[FTK_WIDGET_INSENSITIVE].bg.b = 0xe0;
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

int ftk_widget_id(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->id : 0;
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
	event.u.rect.x = priv->left;
	event.u.rect.y = priv->top;
	event.u.rect.width = priv->width;
	event.u.rect.height = priv->height;
		
	return ftk_widget_event(window, &event);
}

FtkCanvas* ftk_widget_canvas(FtkWidget* thiz)
{
	return thiz != NULL && thiz->priv != NULL ? thiz->priv->canvas : NULL;
}

void ftk_widget_move(FtkWidget* thiz, int x, int y)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->left = x;
	thiz->priv->top  = y;

	return;
}

void ftk_widget_resize(FtkWidget* thiz, int width, int height)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->priv->width = width;
	thiz->priv->height = height;

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

	if(visible)
	{
		ftk_widget_paint(thiz);
	}
	else
	{
		/*FIXME: now, as ftk not support clip, repaint the whole parent surface.*/
		ftk_widget_paint(ftk_widget_parent(thiz));
	}

	return;
}

void ftk_widget_set_focused(FtkWidget* thiz, int focused)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->priv->state != FTK_WIDGET_INSENSITIVE)
	{
		thiz->priv->state = focused ? FTK_WIDGET_FOCUSED : FTK_WIDGET_NORMAL;
	}
	
	if(!ftk_widget_is_parent_visible(thiz))
	{
		return;
	}

	ftk_widget_paint_self(thiz);

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
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	thiz->parent = parent;
	ftk_widget_set_canvas(thiz, ftk_widget_canvas(ftk_widget_toplevel(parent)));

	return;
}

void ftk_widget_append_child(FtkWidget* thiz, FtkWidget* child)
{
	return_if_fail(thiz != NULL && thiz->priv != NULL);

	if(thiz->children == NULL)
	{
		thiz->children = child;
	}
	else
	{
		ftk_widget_append_sibling(thiz->children, child);
	}

	ftk_widget_set_parent(child, thiz);

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

	ftk_widget_paint_self(thiz);

	if(ftk_widget_next(thiz) != NULL)
	{
		ftk_widget_paint(ftk_widget_next(thiz));
	}

	return;
}

void    ftk_widget_set_gc(FtkWidget* thiz, FtkWidgetState state, FtkGc* gc)
{
	return_if_fail(thiz != NULL && state < FTK_WIDGET_STATE_NR && gc != NULL);

	ftk_gc_copy(thiz->priv->gc+state, gc);

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
		for(i = 0; i < FTK_WIDGET_STATE_NR; i++)
		{
			ftk_gc_reset(thiz->priv->gc+i);
		}
		FTK_ZFREE(thiz->priv, sizeof(thiz->priv));
		if(thiz->destroy != NULL)
		{
			thiz->destroy(thiz);
		}
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

void ftk_widget_paint_self(FtkWidget* thiz)
{
	return_if_fail(thiz != NULL && thiz->on_paint != NULL);
	
	if(ftk_widget_is_visible(thiz) && ftk_widget_is_parent_visible(thiz))
	{
		FtkWidgetInfo* priv =  thiz->priv;
		FTK_BEGIN_PAINT(x, y, width, height, canvas);
		FtkBitmap* bitmap = priv->gc[priv->state].bitmap;
		
		if(bitmap != NULL)
		{
			ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, width, height, x, y);
		}
		else
		{
			FtkGc gc = {0};
			gc.mask = FTK_GC_FG;
			gc.fg = ftk_widget_get_gc(thiz)->bg;
			ftk_canvas_set_gc(priv->canvas, &gc); 
			ftk_canvas_draw_rect(priv->canvas, x, y, width, height, 1);
		}

		if(thiz->on_paint != NULL)
		{
			thiz->on_paint(thiz);
		}
	
		if(thiz->children != NULL)
		{
			ftk_widget_paint(thiz->children);
		}
	}
	
	return;
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
