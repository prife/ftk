/*
 * File: ftk_image.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: image control.  
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

#include "ftk_image.h"
#include "ftk_globals.h"

typedef struct _PrivInfo
{
	FtkBitmap* bitmap;
}PrivInfo;

static Ret ftk_image_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_image_on_paint(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	
	ftk_canvas_set_gc(canvas, ftk_widget_get_gc(thiz)); 
	if(priv->bitmap != NULL)
	{
		ftk_canvas_draw_bitmap(canvas, priv->bitmap, 0, 0, width, height, x, y);
	}
	else
	{
		ftk_canvas_draw_rect(canvas, x, y, width, height, 1);
	}

	FTK_END_PAINT();
}

static void ftk_image_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		ftk_bitmap_unref(priv->bitmap);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_image_create(int id, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		thiz->on_event = ftk_image_on_event;
		thiz->on_paint = ftk_image_on_paint;
		thiz->destroy  = ftk_image_destroy;

		ftk_widget_init(thiz, FTK_IMAGE, id);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
		ftk_widget_set_insensitive(thiz, 1);
	}

	return thiz;
}

Ret ftk_image_set_image_file(FtkWidget* thiz, const char* image_file)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && image_file != NULL, RET_FAIL);

	if(priv->bitmap != NULL)
	{
		ftk_bitmap_unref(priv->bitmap);
	}

	priv->bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), image_file);
	ftk_widget_paint_self(thiz);

	return RET_OK;
}

