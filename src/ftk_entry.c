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
typedef struct _PrivInfo
{
	int   caret;
	char* text;
	char* preedit_text;
}PrivInfo;

#define FTK_ENTRY_LEFT_MARGIN 3
#define FTK_ENTRY_TOP_MARGIN  3

static Ret ftk_entry_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_entry_on_paint(FtkWidget* thiz)
{
	FtkGc gc = {.mask = FTK_GC_FG};
	DECL_PRIV0(thiz, priv);
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
		ftk_canvas_draw_string(canvas, x + FTK_ENTRY_LEFT_MARGIN, y+ftk_canvas_font_height(canvas), priv->text);
	}

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
	}

	return thiz;
}

Ret ftk_entry_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	priv->text = strdup(text);
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_entry_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}


