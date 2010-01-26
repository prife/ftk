/*
 * File: ftk_list_render_default.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   default list render
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_globals.h"
#include "ftk_icon_cache.h"
#include "ftk_list_model_default.h"
#include "ftk_list_render_default.h"

typedef struct _PrivInfo
{
	FtkBitmap* more;
	FtkBitmap* radio_off;
	FtkBitmap* radio_on;
	FtkBitmap* check_off;
	FtkBitmap* check_on;
	FtkListModel* model;
}PrivInfo;

static Ret ftk_list_render_default_init(FtkListRender* thiz, FtkListModel* model)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL && model != NULL, RET_FAIL);

	priv->model = model;

	return RET_OK;
}

static Ret ftk_list_render_default_paint(FtkListRender* thiz, FtkCanvas* canvas, int pos, int x, int y, int w, int h)
{
	int dx = 0;
	int dy = 0;
	DECL_PRIV(thiz, priv);
	FtkBitmap* right_icon = NULL;
	FtkListItemInfo* info = NULL;
	return_val_if_fail(thiz != NULL && canvas != NULL && w > 0 && h > 0, RET_FAIL);
	ftk_list_model_get_data(priv->model, pos, (void**)&info);
	return_val_if_fail(info != NULL, RET_FAIL);

	if(info->left_icon != NULL)
	{
		ftk_canvas_draw_bg_image(canvas, info->left_icon, FTK_BG_CENTER, x, y, h, h);
	}
	
	switch(info->type)
	{
		case FTK_LIST_ITEM_RADIO:
		{
			right_icon = info->state ? priv->radio_on : priv->radio_off;
			break;
		}
		case FTK_LIST_ITEM_CHECK:
		{
			right_icon = info->state ? priv->check_on : priv->check_off;
			break;
		}
		case FTK_LIST_ITEM_MORE:
		{
			right_icon = priv->more;
			break;
		}
		default:
		{
			right_icon = info->right_icon;
			break;
		}
	}

	if(right_icon != NULL)
	{
		ftk_canvas_draw_bg_image(canvas, right_icon, FTK_BG_CENTER, x+w-h, y, h, h);
	}

	if(info->text != NULL)
	{
		int text_width = w;
		const char* end = NULL;
		text_width = info->left_icon != NULL  ? text_width - h : text_width;
		text_width = right_icon != NULL ? text_width - h : text_width;
		
		dy = y + FTK_HALF(h);
		dx = FTK_H_MARGIN + (info->left_icon != NULL  ? x + h : x);

		end = ftk_canvas_calc_str_visible_range(canvas, info->text, 0, -1, text_width);
		ftk_canvas_draw_string_ex(canvas, dx, dy, info->text, end - info->text, 1);
	}

	return RET_OK;
}

static void ftk_list_render_default_destroy(FtkListRender* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		ftk_bitmap_unref(priv->radio_on);
		ftk_bitmap_unref(priv->radio_off);
		ftk_bitmap_unref(priv->check_on);
		ftk_bitmap_unref(priv->check_off);
		ftk_bitmap_unref(priv->more);

		FTK_ZFREE(thiz, sizeof(FtkListRender) + sizeof(PrivInfo));
	}

	return;
}

FtkListRender* ftk_list_render_default_create(void)
{
	FtkListRender* thiz = FTK_ZALLOC(sizeof(FtkListRender) + sizeof(PrivInfo));
	
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->init    = ftk_list_render_default_init;
		thiz->paint   = ftk_list_render_default_paint;
		thiz->destroy = ftk_list_render_default_destroy;

		priv->radio_off = ftk_theme_load_image(ftk_default_theme(), "btn_radio_off"FTK_STOCK_IMG_SUFFIX);
		priv->radio_on = ftk_theme_load_image(ftk_default_theme(),  "btn_radio_on"FTK_STOCK_IMG_SUFFIX);
		priv->check_off = ftk_theme_load_image(ftk_default_theme(), "btn_check_off"FTK_STOCK_IMG_SUFFIX);
		priv->check_on = ftk_theme_load_image(ftk_default_theme(),  "btn_check_on"FTK_STOCK_IMG_SUFFIX);
		priv->more = ftk_theme_load_image(ftk_default_theme(),      "more"FTK_STOCK_IMG_SUFFIX);
	}

	return thiz;
}

