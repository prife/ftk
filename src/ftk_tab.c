/*
 * File: ftk_tab.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   tab widget
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
 * 2010-09-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_tab.h"
#include "ftk_theme.h"
#include "ftk_globals.h"
#include "ftk_window.h"

typedef struct _TabPage
{
	char* text;
	FtkBitmap* icon;
	FtkWidget* page;
}TabPage;

typedef struct _TabPrivInfo
{
	TabPage* pages;

	int tab_width;
	int tab_height;
	int active_page;
	int page_use_nr;
	int page_total_nr;
	int mouse_pressed;

	int style;//Vertical, Horizon
	FtkBitmap* normal;
	FtkBitmap* pressed;
	FtkBitmap* selected;
	FtkBitmap* bar_pressed;
	FtkBitmap* bar_selected;
}PrivInfo;

#define FTK_TAB_HANDLE_WIDTH     100
#define FTK_TAB_HANDLE_MIN_WIDTH 48
#define FTK_TAB_HANDLE_MAX_WIDTH 120
#define FTK_TAB_HANDLE_HEIGHT     48

static Ret ftk_tab_page_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_tab_page_on_paint(FtkWidget* thiz)
{

	return RET_OK;
}

static void ftk_tab_page_destroy(FtkWidget* thiz)
{
	return;
}

static FtkWidget* ftk_tab_page_create_internal(FtkWidget* parent)
{
	int x=0, y=0;
	DECL_PRIV0(parent, priv);
	int width = ftk_widget_width(parent);
	int height = ftk_widget_height(parent);
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	switch(priv->style)
	{
	case FTK_TABPAGE_TOP:
		y = priv->tab_height + ftk_bitmap_height(priv->bar_pressed);
		height -= y;
		break;
	case FTK_TABPAGE_LEFT:
		x = priv->tab_width + ftk_bitmap_height(priv->bar_pressed);
		width -= x;
		break;
	case FTK_TABPAGE_RIGHT:
		x = priv->tab_width+ ftk_bitmap_height(priv->bar_pressed);
		width -= x;
		x = 0;
		break;
	case FTK_TABPAGE_BOTTOM:
		y = priv->tab_height + ftk_bitmap_height(priv->bar_pressed);
		height -= y;
		y = 0;
		break;
	}
	thiz->on_event = ftk_tab_page_on_event;
	thiz->on_paint = ftk_tab_page_on_paint;
	thiz->destroy = ftk_tab_page_destroy;

	ftk_widget_init(thiz, FTK_TAB_PAGE, 0, x, y, width, height,
		FTK_ATTR_NO_FOCUS);
	ftk_widget_append_child(parent, thiz);

	return thiz;
}

#if 0
//TODO: fix bug
static Ret ftk_tab_page_tab_calc_width(FtkWidget* thiz)
{
	int len = 0;
	DECL_PRIV0(thiz, priv);

	if(priv->page_total_nr > 0)
	{
		len = ftk_widget_width(thiz)/priv->page_use_nr;
		len = FTK_MAX(len, FTK_TAB_HANDLE_MIN_WIDTH);
		len = FTK_MIN(len, FTK_TAB_HANDLE_MAX_WIDTH);
		priv->tab_width = len;

		priv->tab_height = FTK_TAB_HANDLE_HEIGHT;
	}

	return RET_OK;
}
#endif

static Ret ftk_tab_on_event(FtkWidget* thiz, FtkEvent* event)
{
	int i = 0;
	Ret ret = RET_OK;
	int active_page = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && event != NULL, RET_OK);

	if(priv->page_use_nr <= 0)
	{
		return RET_OK;
	}

	if(event->type == FTK_EVT_SHOW)
	{
		for(i = 0; i < priv->page_use_nr; i++)
		{
			ftk_widget_show_all(priv->pages[i].page, i == priv->active_page);
		}

		return RET_OK;
	}

	active_page = priv->active_page;
	if(event->type == FTK_EVT_MOVE_RESIZE || event->type == FTK_EVT_RESIZE)
	{
		int i = 0;
		int y = priv->tab_height + ftk_bitmap_height(priv->bar_pressed);
		int w = ftk_widget_width(thiz);
		int h = ftk_widget_height(thiz) - y;

		for(i = 0; i < priv->page_use_nr; i++)
		{
			ftk_widget_move_resize(priv->pages[i].page, 0, y, w, h);
		}
		return RET_OK;
	}

	if(event->type == FTK_EVT_KEY_DOWN)
	{
		return RET_REMOVE;
	}
	else if(event->type == FTK_EVT_KEY_UP)
	{
		if(event->u.key.code == FTK_KEY_LEFT)
		{
			active_page--;
		}
		else if(event->u.key.code == FTK_KEY_RIGHT)
		{
			active_page++;
		}
		else if(event->u.key.code == FTK_KEY_TAB || event->u.key.code == FTK_KEY_DOWN)
		{
			TabPage* page = priv->pages+active_page;
			if(page != NULL && page->page->children != NULL)
			{
				ftk_window_set_focus(ftk_widget_toplevel(thiz), page->page->children);
			}
			return RET_REMOVE;
		}
		active_page = (active_page + priv->page_use_nr)%priv->page_use_nr;
	}
	else if(event->type == FTK_EVT_MOUSE_UP || event->type == FTK_EVT_MOUSE_DOWN)
	{
		int ox, oy;

		priv->mouse_pressed = 0;
		ox = event->u.mouse.x - ftk_widget_left_abs(thiz);
		oy = event->u.mouse.y - ftk_widget_top_abs(thiz);
		switch(priv->style)
		{
		case FTK_TABPAGE_TOP:
			if(oy <= priv->tab_height && ox < (priv->page_use_nr * priv->tab_width))
			{
				active_page = ox / priv->tab_width;
				priv->mouse_pressed = event->type == FTK_EVT_MOUSE_DOWN;
			}
			break;
		case FTK_TABPAGE_LEFT:
			if(ox <= priv->tab_width && oy < (priv->page_use_nr * priv->tab_height))
			{
				active_page = oy / priv->tab_height;
				priv->mouse_pressed = event->type == FTK_EVT_MOUSE_DOWN;
			}
			break;
		case FTK_TABPAGE_RIGHT:
			if(ox >= (ftk_widget_width(thiz) - priv->tab_width) && oy < (priv->page_use_nr * priv->tab_height))
			{
				active_page = oy / priv->tab_height;
				priv->mouse_pressed = event->type == FTK_EVT_MOUSE_DOWN;
			}
			break;
		case FTK_TABPAGE_BOTTOM:
			if(oy >= (ftk_widget_height(thiz) - priv->tab_height) && ox < (priv->page_use_nr * priv->tab_width))
			{
				active_page = ox / priv->tab_width;
				priv->mouse_pressed = event->type == FTK_EVT_MOUSE_DOWN;
			}
			break;
		}

		if(active_page == priv->active_page)
		{
			ftk_widget_invalidate(thiz);
		}
	}

	if(active_page != priv->active_page)
	{
		ret = RET_REMOVE;
		ftk_tab_set_active_page(thiz, active_page);
	}

	return ret;
}

static Ret ftk_tab_paint_one_tab(FtkWidget* thiz, int index)
{
	int w = 0;
	int ox = 0;
	int oy = 0;
	DECL_PRIV0(thiz, priv);
	FtkTextLine line = {0};
	FtkBitmap* bitmap = NULL;
	TabPage* page = priv->pages+index;
	FtkTextLayout* text_layout = ftk_default_text_layout();
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	(void)height;
	(void)width;

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz));
	if(index != priv->active_page)
	{
		bitmap = priv->normal;
	}
	else
	{
		bitmap = priv->mouse_pressed ? priv->pressed : priv->selected;
	}

	//draw tab
	switch(priv->style)
	{
	case FTK_TABPAGE_TOP:
		ox = x + index * priv->tab_width;
		oy = y;
		break;
	case FTK_TABPAGE_LEFT:
		ox = x;
		oy = y + index * priv->tab_height;
		break;
	case FTK_TABPAGE_RIGHT:
		ox = x + ftk_widget_width(thiz) - priv->tab_width;
		oy = y + index * priv->tab_height;
		break;
	case FTK_TABPAGE_BOTTOM:
		ox = x + index * priv->tab_width;
		oy = y + ftk_widget_height(thiz) - priv->tab_height;
		break;
	}

	if (bitmap != NULL)
	{
		ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_FOUR_CORNER,
			ox, oy, priv->tab_width, priv->tab_height);
	}

	w = priv->tab_width;
	if(page->icon != NULL)
	{
		int ty;
		ty = oy + FTK_HALF(priv->tab_height - ftk_bitmap_height(page->icon));
		ftk_canvas_draw_bitmap_simple(canvas, page->icon, 0, 0,
			ftk_bitmap_width(page->icon), ftk_bitmap_height(page->icon), ox, ty);

		w -= ftk_bitmap_width(page->icon);
		ox += ftk_bitmap_width(page->icon);
	}

	if(page->text != NULL)
	{
		const char* text = page->text;
		ftk_text_layout_init(text_layout, text, -1, canvas, w);

		oy = oy + FTK_HALF(priv->tab_height);
		if(ftk_text_layout_get_visual_line(text_layout, &line) == RET_OK)
		{
			ox = ox + FTK_HALF(w - line.extent);
			ftk_canvas_draw_string(canvas, ox, oy, text, -1, 1);
		}
	}

	return RET_OK;
}

static Ret ftk_tab_on_paint(FtkWidget* thiz)
{
	int i = 0;
	DECL_PRIV0(thiz, priv);
	FtkBitmap* bitmap = NULL;
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	return_val_if_fail(priv != NULL, RET_FAIL);

	if(priv->page_use_nr > 0)
	{
		(void)height;
		for(i = 0; i < priv->page_use_nr; i++)
		{
			if(i == priv->active_page) continue;
			ftk_tab_paint_one_tab(thiz, i);
		}
		ftk_tab_paint_one_tab(thiz, priv->active_page);
	}

#if 0
	//TODO: never be here! need to be cleaned
	else
	{
		ftk_canvas_reset_gc(canvas, ftk_widget_get_gc(thiz));
		ftk_canvas_draw_rect(canvas, x, y, width, height, 0, 0);
	}

	//TODO: draw the small split line
	bitmap = priv->mouse_pressed ? priv->bar_pressed : priv->bar_selected;
	ftk_canvas_draw_bg_image(canvas, bitmap, FTK_BG_FOUR_CORNER,
		x, y + FTK_TAB_HANDLE_HEIGHT, width, ftk_bitmap_height(bitmap));
#endif

	FTK_END_PAINT();
}

static void ftk_tab_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		int i = 0;
		TabPage* iter = NULL;
		DECL_PRIV0(thiz, priv);
		FTK_BITMAP_UNREF(priv->normal);
		FTK_BITMAP_UNREF(priv->pressed);
		FTK_BITMAP_UNREF(priv->selected);
		FTK_BITMAP_UNREF(priv->bar_selected);
		FTK_BITMAP_UNREF(priv->bar_pressed);

		for(i = 0; i < priv->page_use_nr; i++)
		{
			iter = priv->pages+i;
			FTK_FREE(iter->text);
			FTK_BITMAP_UNREF(iter->icon);
		}
		FTK_FREE(priv->pages);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_tab_create_ex(FtkWidget* parent,
        int x, int y, int width, int height,
        int tab_width, int tab_height, int style)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		DECL_PRIV0(thiz, priv);
		thiz->on_event = ftk_tab_on_event;
		thiz->on_paint = ftk_tab_on_paint;
		thiz->destroy  = ftk_tab_destroy;

		ftk_widget_init(thiz, FTK_TAB, 0,
			x, y, width, height, FTK_ATTR_TRANSPARENT|FTK_ATTR_BG_FOUR_CORNER);
		ftk_widget_append_child(parent, thiz);

		priv->tab_width = tab_width;
		priv->tab_height = tab_height;

		priv->style = (style < FTK_TABPAGE_TOP || style > FTK_TABPAGE_RIGHT) ?
			FTK_TABPAGE_LEFT : style;

		priv->pressed = ftk_theme_load_image(ftk_default_theme(),
			"tab-pressed"FTK_STOCK_IMG_SUFFIX);
		priv->selected = ftk_theme_load_image(ftk_default_theme(),
			"tab-selected"FTK_STOCK_IMG_SUFFIX);
		priv->normal = ftk_theme_load_image(ftk_default_theme(),
			"tab-normal"FTK_STOCK_IMG_SUFFIX);

		priv->bar_pressed = ftk_theme_load_image(ftk_default_theme(),
			"tab-bar-pressed"FTK_STOCK_IMG_SUFFIX);
		priv->bar_selected = ftk_theme_load_image(ftk_default_theme(),
			"tab-bar-selected"FTK_STOCK_IMG_SUFFIX);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

FtkWidget* ftk_tab_create(FtkWidget* parent, int x, int y, int width, int height)
{
	return ftk_tab_create_ex(parent, x, y, width, height,
            FTK_TAB_HANDLE_WIDTH, FTK_TAB_HANDLE_HEIGHT, FTK_TABPAGE_LEFT);
}

/* return the tab_width, not the widget length */
int ftk_tab_get_width(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, -1);
	return priv->tab_width;
}

/* return the tab_height, not the widget height */
int ftk_tab_get_height(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, -1);
	return priv->tab_height;
}

int ftb_tab_set_img(FtkWidget* thiz, FtkBitmap* bitmap, int which)
{
	DECL_PRIV0(thiz, priv);
	FtkBitmap** origin = NULL;
	return_val_if_fail(priv != NULL, -1);
	switch (which)
	{
	case FTK_TAB_IMG_NORMAL:
		origin = &priv->normal;
		break;
	case FTK_TAB_IMG_PRESSED:
		origin = &priv->pressed;
		break;
	case FTK_TAB_IMG_SELECTED:
		origin = &priv->selected;
		break;
	case FTK_TAB_IMG_BAR_SELECTED:
		origin = &priv->bar_selected;
		break;
	case FTK_TAB_IMG_BAR_PRESSED:
		origin = &priv->bar_pressed;
		break;
	default:
		break;
	}

	if (origin)
	{
		FTK_BITMAP_UNREF(*origin);
		*origin = bitmap;
	}

	return 0;
}

int ftk_tab_get_page_count(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->page_use_nr;
}

FtkWidget* ftk_tab_get_page(FtkWidget* thiz, int index)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, NULL);

	return priv->pages[index].page;
}

int ftk_tab_get_page_index(FtkWidget* thiz, FtkWidget* page)
{
	int i = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, -1);

	for(i = 0; i < priv->page_use_nr; i++)
	{
		if(priv->pages[i].page == page)
		{
			return i;
		}
	}

	return -1;
}

Ret ftk_tab_remove_page(FtkWidget* thiz, int index)
{
	int i = 0;
	TabPage* iter = NULL;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, RET_FAIL);

	iter = priv->pages+index;

	FTK_FREE(iter->text);
	FTK_BITMAP_UNREF(iter->icon);
	ftk_widget_remove_child(thiz, iter->page);

	for(i = index; (i + 1) < priv->page_use_nr; i++)
	{
		priv->pages[i] = priv->pages[i+1];
	}
	priv->page_use_nr--;
	//ftk_tab_page_tab_calc_width(thiz);

	return RET_OK;
}

FtkWidget* ftk_tab_add_page(FtkWidget* thiz, const char* text, FtkBitmap* icon)
{
	TabPage* iter = NULL;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && text != NULL, NULL);

	if((priv->page_use_nr + 1) > priv->page_total_nr)
	{
		int page_total_nr = priv->page_total_nr + 3;
		TabPage* pages = (TabPage*)FTK_REALLOC(priv->pages, page_total_nr * sizeof(TabPage));
		if(pages != NULL)
		{
			priv->pages = pages;
			priv->page_total_nr = page_total_nr;
		}
	}

	return_val_if_fail((priv->page_use_nr + 1) <= priv->page_total_nr, NULL);

	iter = priv->pages + priv->page_use_nr;
	priv->page_use_nr++;

	iter->icon = icon;
	iter->text = FTK_STRDUP(text);
	if(icon != NULL)
	{
		ftk_bitmap_ref(icon);
	}
	//ftk_tab_page_tab_calc_width(thiz);
	iter->page = ftk_tab_page_create_internal(thiz);
	ftk_tab_set_active_page(thiz, priv->page_use_nr - 1);

	return iter->page;
}

int ftk_tab_get_active_page(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->active_page;
}

Ret ftk_tab_set_active_page(FtkWidget* thiz, int index)
{
	int i = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, RET_FAIL);

	if(priv->active_page != index)
	{
		FtkEvent event;
		int old = priv->active_page;

		priv->active_page = index;
		memset(&event, 0x00, sizeof(event));

		ftk_event_init(&event, FTK_EVT_TAB_PAGE_DEACTIVATE);
		event.widget = priv->pages[old].page;
		ftk_widget_event(thiz, &event);

		ftk_event_init(&event, FTK_EVT_TAB_PAGE_ACTIVATE);
		event.widget = priv->pages[index].page;
		ftk_widget_event(thiz, &event);

		for(i = 0; i < priv->page_use_nr; i++)
		{
			ftk_widget_show_all(priv->pages[i].page, i == priv->active_page);
		}

		ftk_widget_invalidate(thiz);
	}

	return RET_OK;
}

Ret ftk_tab_set_page_text(FtkWidget* thiz, int index, const char* text)
{
	TabPage* iter = NULL;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, RET_FAIL);

	iter = priv->pages+index;
	FTK_FREE(iter->text);
	iter->text = FTK_STRDUP(text);

	return RET_OK;
}

Ret ftk_tab_set_page_icon(FtkWidget* thiz, int index, FtkBitmap* icon)
{
	TabPage* iter = NULL;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, RET_FAIL);

	iter = priv->pages+index;
	FTK_BITMAP_UNREF(iter->icon);
	iter->icon = icon;
	if(icon != NULL)
	{
		ftk_bitmap_ref(icon);
	}

	return RET_OK;
}

const char* ftk_tab_get_page_text(FtkWidget* thiz, int index)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, NULL);

	return priv->pages[index].text;
}

FtkBitmap* ftk_tab_get_page_icon(FtkWidget* thiz, int index)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(priv != NULL && index < priv->page_use_nr, NULL);

	return priv->pages[index].icon;
}

FtkWidget* ftk_tab_page_create(FtkWidget* parent, int x, int y, int width, int height)
{
	return_val_if_fail(ftk_widget_type(parent) == FTK_TAB, NULL);

	return ftk_tab_add_page(parent, "", NULL);
}

