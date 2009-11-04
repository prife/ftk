/*
 * File: ftk_entry.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   single enditor
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
 * 2009-11-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_style.h"
#include "ftk_entry.h"
#include "ftk_globals.h"

typedef struct _PrivInfo
{
	int   caret;
	int   caret_show;
	int   visible_start;
	int   visible_end;
	char* text;
	char* preedit_text;
	FtkSource* caret_timer;
}PrivInfo;

#define FTK_ENTRY_LEFT_MARGIN 4
#define FTK_ENTRY_TOP_MARGIN  4

static Ret ftk_entry_on_paint_caret(FtkWidget* thiz);
static Ret ftk_entry_compute_visible_range(FtkWidget* thiz);

static Ret ftk_entry_move_caret(FtkWidget* thiz, int offset)
{
	DECL_PRIV0(thiz, priv);
	priv->caret_show = 0;
	ftk_entry_on_paint_caret(thiz);
	return_val_if_fail(priv->text != NULL, RET_OK);

	/*FIXME: CJK*/
	priv->caret += offset;
	priv->caret = priv->caret < 0 ? 0 : priv->caret;
	priv->caret = priv->caret > strlen(priv->text) ? strlen(priv->text) : priv->caret;

	if((priv->caret) < priv->visible_start)
	{
		priv->visible_start = priv->caret;
		priv->visible_end = -1;
	}
	else if((priv->caret) > priv->visible_end)
	{
		priv->visible_end = priv->caret;
		priv->visible_start = -1;
	}

	ftk_entry_compute_visible_range(thiz);

	return RET_OK;
}

static Ret ftk_entry_handle_key_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV0(thiz, priv);
	switch(event->u.key.code)
	{
		case FTK_KEY_LEFT:
		{
			ftk_entry_move_caret(thiz, -1);
			break;
		}
		case FTK_KEY_RIGHT:
		{
			ftk_entry_move_caret(thiz, 1);
			break;
		}
	}
	return RET_REMOVE;
}

static Ret ftk_entry_on_event(FtkWidget* thiz, FtkEvent* event)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	switch(event->type)
	{
		case FTK_EVT_FOCUS_IN:
		{
			ftk_source_ref(priv->caret_timer);
			ftk_main_loop_add_source(ftk_default_main_loop(), priv->caret_timer);
			break;
		}
		case FTK_EVT_FOCUS_OUT:
		{
			ftk_main_loop_remove_source(ftk_default_main_loop(), priv->caret_timer);
			break;
		}
	//	case FTK_EVT_KEY_DOWN:
		case FTK_EVT_KEY_UP:
		{
			return ftk_entry_handle_key_event(thiz, event);
			break;
		}
		default:break;
	}
	return RET_OK;
}

static Ret ftk_entry_on_paint_caret(FtkWidget* thiz)
{
	int extent = 0;
	DECL_PRIV0(thiz, priv);
	FtkGc gc = {.mask = FTK_GC_FG};
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	ftk_entry_compute_visible_range(thiz);
	return_val_if_fail(priv->caret >= priv->visible_start && priv->caret <= priv->visible_end, RET_FAIL);

	if(ftk_widget_is_focused(thiz))
	{
		gc.fg = priv->caret_show ? ftk_widget_get_gc(thiz)->fg : ftk_widget_get_gc(thiz)->bg;
		priv->caret_show = !priv->caret_show;

		extent = ftk_canvas_get_extent(canvas, priv->text+priv->visible_start, priv->caret - priv->visible_start);

		ftk_canvas_reset_gc(canvas, &gc);
		x += extent + FTK_ENTRY_LEFT_MARGIN - 1;
		y += FTK_ENTRY_TOP_MARGIN;
		ftk_canvas_draw_vline(canvas, x, y, height - 2 * FTK_ENTRY_TOP_MARGIN);
		FTK_END_PAINT();
	}

	return RET_OK;
}

static Ret ftk_entry_on_paint(FtkWidget* thiz)
{
	FtkGc gc = {.mask = FTK_GC_FG};
	DECL_PRIV0(thiz, priv);
	int font_height = 0;
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	
	if(ftk_widget_is_focused(thiz))
	{
		gc.fg = ftk_style_get_color(FTK_COLOR_HIGHLIGHT);
	}
	else
	{
		gc.fg = ftk_style_get_color(FTK_COLOR_BORDER);
	}

	ftk_canvas_set_gc(canvas, &gc);
	ftk_canvas_draw_hline(canvas, x + 2, y, width-4);
	ftk_canvas_draw_hline(canvas, x + 1, y + 1, width-2);
	ftk_canvas_draw_vline(canvas, x, y + 2, height - 4);
	ftk_canvas_draw_vline(canvas, x + 1, y + 1, height - 2);
	
	ftk_canvas_draw_vline(canvas, x+width-1, y + 2, height - 4);
	ftk_canvas_draw_vline(canvas, x + width -2, y + 1, height - 2);
	ftk_canvas_draw_hline(canvas, x + 1, y + height - 1, width-2);
	ftk_canvas_draw_hline(canvas, x + 2, y + height - 2, width-4);

	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz)); 
	if(priv->text != NULL)
	{
		ftk_entry_compute_visible_range(thiz);
		font_height = ftk_canvas_font_height(canvas);
		x += FTK_ENTRY_LEFT_MARGIN;
		y += font_height ;

		ftk_canvas_draw_string(canvas, x, y, priv->text + priv->visible_start,
			priv->visible_end - priv->visible_start);
	}
	ftk_entry_on_paint_caret(thiz);

	FTK_END_PAINT();
}

static void ftk_entry_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);

		FTK_FREE(priv->text);
		FTK_FREE(priv->preedit_text);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_entry_create(int id, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		FtkGc gc = {.mask = FTK_GC_FG | FTK_GC_BG};
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
		DECL_PRIV0(thiz, priv);

		thiz->on_event = ftk_entry_on_event;
		thiz->on_paint = ftk_entry_on_paint;
		thiz->destroy  = ftk_entry_destroy;

		height = ftk_font_height(ftk_default_font()) + FTK_ENTRY_TOP_MARGIN * 2;
		ftk_widget_init(thiz, FTK_ENTRY, id);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);

		gc.fg = ftk_style_get_color(FTK_COLOR_WINDOWTEXT);
		gc.bg = ftk_style_get_color(FTK_COLOR_WINDOW);
		ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(thiz, FTK_WIDGET_FOCUSED, &gc);
		
		gc.fg = ftk_style_get_color(FTK_COLOR_GRAYTEXT);
		ftk_widget_set_gc(thiz, FTK_WIDGET_INSENSITIVE, &gc);
		priv->caret_timer = ftk_source_timer_create(500, ftk_widget_paint_self, thiz);
	}

	return thiz;
}

static Ret ftk_entry_compute_visible_range(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	int width = ftk_widget_width(thiz);
	FtkCanvas* canvas = ftk_widget_canvas(thiz);
	const char* other_side = NULL;
	return_val_if_fail(thiz != NULL && priv->text != NULL, RET_FAIL);

	if(priv->visible_start < 0 || priv->visible_end < 0)
	{
		other_side = ftk_canvas_compute_string_visible_ranage(canvas, priv->text, 
			priv->visible_start, priv->visible_end, width);

		if(priv->visible_start < 0)
		{
			priv->visible_start = other_side - priv->text;
		}
		else if(priv->visible_end < 0)
		{
			priv->visible_end = other_side - priv->text;
		}
	}

	return RET_OK;
}

Ret ftk_entry_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	priv->text = strdup(text);
	
	priv->visible_start = -1;
	priv->visible_end = strlen(priv->text);
	priv->caret = priv->visible_end;
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_entry_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}


