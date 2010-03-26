/*
 * File: ftk_display_mem.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   memory based display.
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
 * 2010-03-21 Li XianJing <xianjimli@hotmail.com> from ftk_display_fb.c
 *
 */

#include "ftk_log.h"
#include "ftk_display_mem.h"

typedef struct _PrivInfo
{
	int width;
	int height;
	void* bits;
	FtkDestroy on_destroy;
	void* on_destroy_ctx;
	FtkPixelFormat format;
	FtkBitmapCopyFromData copy_from_data;
	FtkBitmapCopyToData   copy_to_data;
}PrivInfo;

static Ret ftk_display_men_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	DECL_PRIV(thiz, priv);
	int display_width  = priv->width;
	int display_height = priv->height;
	return_val_if_fail(priv != NULL, RET_FAIL);

	ftk_logd("%s: ox=%d oy=%d (%d %d %d %d)\n", __func__, xoffset, yoffset, 
		rect->x, rect->y, rect->width, rect->height);
	return priv->copy_to_data(bitmap, rect, 
		priv->bits, xoffset, yoffset, display_width, display_height); 
}

static int ftk_display_men_width(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->width;
}

static int ftk_display_men_height(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, 0);

	return priv->height;
}

static Ret ftk_display_men_snap(FtkDisplay* thiz, size_t x, size_t y, FtkBitmap* bitmap)
{
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	int w = ftk_display_width(thiz);
	int h = ftk_display_height(thiz);
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	return_val_if_fail(priv != NULL, RET_FAIL);
	
	rect.x = x;
	rect.y = y;
	rect.width = bw;
	rect.height = bh;

	return priv->copy_from_data(bitmap, priv->bits, w, h, &rect);
}

static void ftk_display_men_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		
		if(priv->on_destroy != NULL)
		{
			priv->on_destroy(priv->on_destroy_ctx);
		}

		FTK_ZFREE(thiz, sizeof(FtkDisplay) + sizeof(PrivInfo));
	}

	return;
}

FtkDisplay* ftk_display_men_create(FtkPixelFormat format, 
	int width, int height, void* bits, FtkDestroy on_destroy, void* ctx)
{
	FtkDisplay* thiz = NULL;

	thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->update   = ftk_display_men_update;
		thiz->width    = ftk_display_men_width;
		thiz->height   = ftk_display_men_height;
		thiz->snap     = ftk_display_men_snap;
		thiz->destroy  = ftk_display_men_destroy;

		priv->bits = bits;
		priv->width = width;
		priv->height = height;
		priv->format = format;
		priv->on_destroy = on_destroy;
		priv->on_destroy_ctx = ctx;

		switch(format)
		{
			case FTK_PIXEL_RGB565:
			{
				priv->copy_to_data   = ftk_bitmap_copy_to_data_rgb565;
				priv->copy_from_data = ftk_bitmap_copy_from_data_rgb565;
				break;
			}
			case FTK_PIXEL_BGR24:
			{
				priv->copy_to_data   = ftk_bitmap_copy_to_data_bgr24;
				priv->copy_from_data = ftk_bitmap_copy_from_data_bgr24;
				break;
			}
			case FTK_PIXEL_BGRA32:
			{
				priv->copy_to_data   = ftk_bitmap_copy_to_data_bgra32;
				priv->copy_from_data = ftk_bitmap_copy_from_data_bgra32;
				break;
			}
			case FTK_PIXEL_RGBA32:
			{
				priv->copy_to_data   = ftk_bitmap_copy_to_data_rgba32;
				priv->copy_from_data = ftk_bitmap_copy_from_data_rgba32;
				break;
			}
			default:
			{
				assert(!"not supported framebuffer format.");
				break;
			}
		}
	}
		
	return thiz;
}

