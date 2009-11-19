/*
 * File: ftk_wait_box.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   wait box 
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
 * 2009-11-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_wait_box.h"

typedef struct _PrivInfo
{
	int index;
	FtkSource* timer;
}PrivInfo;

static Ret ftk_wait_box_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RTE_OK;
}

static const char** images[] = 
{
	"wait0"FTK_STOCK_IMG_SUFFIX,
	"wait1"FTK_STOCK_IMG_SUFFIX,
	"wait2"FTK_STOCK_IMG_SUFFIX,
	"wait3"FTK_STOCK_IMG_SUFFIX,
};

static Ret ftk_wait_box_on_paint(FtkWidget* thiz)
{
	FtkBitmap* bitmap = NULL;
	DECL_PRIV0(thiz, priv);
	FTK_BEGIN_PAINT(x, y, width, height, canvas);
	priv->index = priv->index %s (sizeof(images)/sizeof(images[0]));

	bitmap = ftk_icon_cache_load(ftk_default_icon_cache(), images[priv->index]);

	priv->index++;
	FTK_END_PAINT();
}

static void ftk_wait_box_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV0(thiz, priv);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

FtkWidget* ftk_wait_box_create(int id, int x, int y, int w, int h)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));

	if(thiz != NULL)
	{
		FtkGc gc = {.mask = FTK_GC_FG | FTK_GC_BG};
		thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));

		thiz->on_event = ftk_wait_box_on_event;
		thiz->on_paint = ftk_wait_box_on_paint;
		thiz->destroy  = ftk_wait_box_destroy;

		ftk_widget_init(thiz, FTK_WAIT_BOX, id);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
	}

	return thiz;
}

Ret ftk_wait_box_start_waiting(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);

	return RET_OK;
}

Ret ftk_wait_box_stop_waiting(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);

	return RET_OK;
}

