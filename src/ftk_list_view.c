/*
 * File: ftk_list_view.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   list view
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

#include "ftk_key.h"
#include "ftk_event.h"
#include "ftk_globals.h"
#include "ftk_window.h"
#include "ftk_list_view.h"
#include "ftk_scroll_bar.h"
typedef struct _PrivInfo
{
	int selected;
	int visible_nr;
	int item_height;
	int visible_start;
	int is_active;
	int top_margin;
	int botton_margin;
	FtkListModel*  model;
	FtkListRender* render;
	FtkWidget* vscroll_bar;
	FtkListener listener;
	void* listener_ctx;
}PrivInfo;

#define FTK_LIST_VIEW_H_MARGIN 2
#define FTK_SCROLL_BAR_WIDTH 10

static Ret ftk_list_view_set_cursor(FtkWidget* thiz, int selected)
{
	DECL_PRIV0(thiz, priv);
	int total = ftk_list_model_get_total(priv->model);
	if(priv->selected == selected && !priv->is_active)
	{
		return RET_OK;
	}

	priv->selected = selected;

	if(priv->selected < 0) priv->selected = 0;
	if(priv->selected >= total) priv->selected = total - 1;

	if(priv->selected < priv->visible_start)
	{
		priv->visible_start = priv->selected;
	}

	if(priv->selected > (priv->visible_start + priv->visible_nr))
	{
		priv->visible_start = priv->selected - priv->visible_nr;
	}

	ftk_widget_paint_self(thiz);

	return RET_REMOVE;
}

static Ret ftk_list_view_move_cursor(FtkWidget* thiz, int offset)
{
	DECL_PRIV0(thiz, priv);
	int total = ftk_list_model_get_total(priv->model);
	return_val_if_fail(total > 0, RET_FAIL);
	return_val_if_fail(priv->visible_nr > 0, RET_FAIL);

	if(priv->selected == 0 && offset < 0)
	{
		return RET_FAIL;
	}
	else if((priv->selected + 1) == total && offset > 0)
	{
		return RET_FAIL;
	}

	return ftk_list_view_set_cursor(thiz, priv->selected + offset);
}

static Ret ftk_list_view_set_cursor_by_pointer(FtkWidget* thiz, int x, int y)
{
	int selected = 0;
	DECL_PRIV0(thiz, priv);
	int offset = y - ftk_widget_top_abs(thiz) - priv->top_margin;
	if(offset <= 0) return RET_OK;

	selected = priv->visible_start + offset/priv->item_height;

	return ftk_list_view_set_cursor(thiz, selected);
}

static Ret ftk_list_view_on_key_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV0(thiz, priv);
	switch(event->u.key.code)
	{
		case FTK_KEY_DOWN:
		{
			if(event->type == FTK_EVT_KEY_DOWN)
			{
				ret = ftk_list_view_move_cursor(thiz, 1);
			}
			break;
		}
		case FTK_KEY_UP:
		{
			if(event->type == FTK_EVT_KEY_DOWN)
			{
				ret = ftk_list_view_move_cursor(thiz, -1);
			}
			break;
		}
		default:
		{
			if(FTK_IS_ACTIVE_KEY(event->u.key.code))
			{
				if(event->type == FTK_KEY_DOWN)
				{
					priv->is_active = 1;
				}
				else
				{
					FTK_CALL_LISTENER(priv->listener, priv->listener_ctx, thiz);
					priv->is_active = 0;
				}
			}

			break;
		}
	}

	return ret;
}

static Ret ftk_list_view_on_mouse_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV0(thiz, priv);

	switch(event->type)
	{
		case FTK_EVT_MOUSE_DOWN:
		{
			priv->is_active = 1;
			ftk_list_view_set_cursor_by_pointer(thiz, event->u.mouse.x, event->u.mouse.y);
			ftk_window_grab(ftk_widget_toplevel(thiz), thiz);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		{
			ftk_window_ungrab(ftk_widget_toplevel(thiz), thiz);
			if(priv->is_active)
			{
				ret = FTK_CALL_LISTENER(priv->listener, priv->listener_ctx, thiz);
				priv->is_active = 0;
				ftk_list_view_set_cursor_by_pointer(thiz, event->u.mouse.x, event->u.mouse.y);
			}
			break;
		}
		default:break;
	}

	return ret;
}

static Ret ftk_list_view_on_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;

	switch(event->type)
	{
		case FTK_EVT_KEY_UP:
		case FTK_EVT_KEY_DOWN:
		{
			ret = ftk_list_view_on_key_event(thiz, event);
			break;
		}
		case FTK_EVT_MOUSE_UP:
		case FTK_EVT_MOUSE_DOWN:
		{
			ret = ftk_list_view_on_mouse_event(thiz, event);
			break;
		}
		default:break;
	}

	return ret;
}

static Ret ftk_list_view_on_paint(FtkWidget* thiz)
{
	int i = 0;
	int dy = 0;
	DECL_PRIV0(thiz, priv);
	FtkBitmap* bitmap = NULL;
	const char* filename = NULL;
	int total = ftk_list_model_get_total(priv->model);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	(void)height;
	dy = y + priv->top_margin;
	for(i = 0; i < priv->visible_nr; i++)
	{
		if((priv->visible_start + i) >= total)
		{
			break;
		}

		if((priv->visible_start + i) == priv->selected)
		{
			filename = priv->is_active ? "list_selector_background_focus"FTK_STOCK_IMG_SUFFIX 
				: "list_selector_background_pressed"FTK_STOCK_IMG_SUFFIX;		
		}
		else
		{
			filename = "list_selector_background_disabled"FTK_STOCK_IMG_SUFFIX;
		}
		bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), filename);

		ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_FOUR_CORNER, 
			x + FTK_LIST_VIEW_H_MARGIN, 
			dy, width - 2 * FTK_LIST_VIEW_H_MARGIN - FTK_SCROLL_BAR_WIDTH, priv->item_height);
		ftk_list_render_paint(priv->render, canvas, priv->visible_start + i, x, dy, width, priv->item_height);
		dy += priv->item_height;
	}

	ftk_scroll_bar_set_param(priv->vscroll_bar, priv->visible_start, total, priv->visible_nr);
	ftk_widget_show(priv->vscroll_bar, priv->visible_nr < total);

	FTK_END_PAINT();
}

static void ftk_list_view_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_list_view_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);
	
	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		DECL_PRIV0(thiz, priv);
		thiz->on_event = ftk_list_view_on_event;
		thiz->on_paint = ftk_list_view_on_paint;
		thiz->destroy  = ftk_list_view_destroy;

		ftk_widget_init(thiz, FTK_LIST_VIEW, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_append_child(parent, thiz);

		priv->vscroll_bar = ftk_scroll_bar_create(thiz, width - FTK_SCROLL_BAR_WIDTH, 0, FTK_SCROLL_BAR_WIDTH, height);
	}
	else
	{
		FTK_ZFREE(thiz, sizeof(FtkWidget));
	}

	return thiz;
}

Ret ftk_list_view_init(FtkWidget* thiz, FtkListModel* model, FtkListRender* render, int item_height)
{
	int margin = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && render != NULL && model != NULL && item_height > 0, RET_FAIL);
	
	priv->model       = model;
	priv->render      = render;
	priv->item_height = item_height;

	margin = ftk_widget_height(thiz)%item_height;
	priv->visible_nr = ftk_widget_height(thiz)/item_height;
	return_val_if_fail(priv->visible_nr > 0, RET_FAIL);

	priv->top_margin = FTK_HALF(margin);
	priv->botton_margin = FTK_HALF(margin);
	priv->visible_start = 0;
	priv->selected      = -1;
	priv->is_active = 0;
	
	return RET_OK;
}

int ftk_list_view_get_selected(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, -1);

	return priv->selected;
}

FtkListModel* ftk_list_view_get_model(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, NULL);

	return priv->model;
}

Ret ftk_list_view_set_clicked_listener(FtkWidget* thiz, FtkListener listener, void* ctx)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->listener = listener;
	priv->listener_ctx = ctx;

	return RET_OK;
}

