/*
 * File: ftk_list_render_default.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   default list render
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_list_render_default.h"

typedef struct _PrivInfo
{
	FtkListModel* model;
}PrivInfo;

static Ret ftk_list_render_default_init(FtkListRender* thiz, FtkListModel* model)
{
	return_val_if_fail(thiz != NULL && model != NULL, RET_FAIL);

	return RET_OK;
}

static Ret ftk_list_render_default_paint(FtkListRender* thiz, FtkCanvas* canvas, int pos, int x, int y, int w, int h)
{

	return RET_OK;
}

static void ftk_list_render_default_destroy(FtkListRender* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkListRender) + sizeof(PrivInfo));
	}

	return;
}

FtkListRender* ftk_list_render_default_create(void)
{
	FtkListRender* thiz = FTK_ZALLOC(sizeof(FtkListRender) + sizeof(PrivInfo));
	
	if(thiz != NULL)
	{
		thiz->init    = ftk_list_render_default_init;
		thiz->paint   = ftk_list_render_default_paint;
		thiz->destroy = ftk_list_render_default_destroy;
	}

	return thiz;
}


