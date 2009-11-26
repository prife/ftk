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

#include "ftk_list_view.h"

typedef struct _PrivInfo
{
	int visible_end;
	int visible_start;
	int selected_offset;
	FtkListModel*  model;
	FtkListRender* render;
	FtkWidget* vscroll_bar;
}PrivInfo;

static Ret ftk_list_view_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_list_view_on_paint(FtkWidget* thiz)
{
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

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

FtkWidget* ftk_list_view_create(FtkWidget* parent, int x, int y, int width, int height);
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	return_val_if_fail(thiz != NULL, NULL);
	
	thiz->priv_subclass[0] = (PrivInfo*)FTK_ZALLOC(sizeof(PrivInfo));
	if(thiz->priv_subclass[0] != NULL)
	{
		FtkGc gc ={0};

		thiz->on_event = ftk_list_view_on_event;
		thiz->on_paint = ftk_list_view_on_paint;
		thiz->destroy  = ftk_list_view_destroy;

		ftk_widget_init(thiz, FTK_BUTTON, 0);
		ftk_widget_move(thiz, x, y);
		ftk_widget_resize(thiz, width, height);
	}
	else
	{
		FTK_ZFREE(thiz, sizeof(FtkWidget));
	}

	return thiz;
}

Ret ftk_list_view_set_model(FtkWidget* thiz, FtkListModel* model)
{
	return RET_OK;
}

Ret ftk_list_view_set_render(FtkWidget* thiz, FtkListRender* render, int item_height)
{
	return RET_OK;
}


