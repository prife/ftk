/*
 * File: ftk_label.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: label control.  
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

#include "ftk_label.h"
typedef struct _PrivInfo
{
	char* text;
}PrivInfo;

#define FTK_LABEL_LEFT_MARGIN 3
#define FTK_LABEL_TOP_MARGIN  1

static Ret ftk_label_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_label_on_paint(FtkWidget* thiz)
{
	int i = 0;
	int rows = 0;
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	
	if(priv->text != NULL)
	{
		int start = 0;
		const char* end = NULL;
		
		x += FTK_LABEL_LEFT_MARGIN;
		width -= FTK_LABEL_LEFT_MARGIN * 2;
		rows = height / (ftk_canvas_font_height(canvas) + FTK_LABEL_TOP_MARGIN);
	
		ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz)); 
		for(i = 0; i < rows; i++)
		{
			y += ftk_canvas_font_height(canvas) + FTK_LABEL_TOP_MARGIN;
			end = ftk_canvas_calc_str_visible_range(canvas, priv->text, start, -1, width);
			ftk_canvas_draw_string(canvas, x, y, priv->text + start, end - priv->text - start);
			if(*end == '\0')
			{
				break;
			}
			start = end - priv->text;
		}
	}

	FTK_END_PAINT();
}

static void ftk_label_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);

		FTK_FREE(priv->text);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_label_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		thiz->on_event = ftk_label_on_event;
		thiz->on_paint = ftk_label_on_paint;
		thiz->destroy  = ftk_label_destroy;

		ftk_widget_init(thiz, FTK_LABEL, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_set_insensitive(thiz, 1);
		ftk_widget_set_attr(thiz, FTK_ATTR_TRANSPARENT);
		ftk_widget_append_child(parent, thiz);
	}
	else
	{
		FTK_FREE(thiz);
	}

	return thiz;
}

Ret ftk_label_set_text(FtkWidget* thiz, const char* text)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	FTK_FREE(priv->text);
	priv->text = FTK_STRDUP(text);
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

const char* ftk_label_get_text(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->text;
}


