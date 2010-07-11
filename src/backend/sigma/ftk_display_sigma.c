/*
 * File: ftk_display_sigma.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   sigma implemented display.
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
 * 2009-12-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ftk_log.h"
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "ftk_display_sigma.h"

typedef struct _PrivInfo
{
	int width;
	int height;
	unsigned char* bits;
	struct osd_descriptor* p_osd;
	int bits_per_pixel;
	FtkBitmapCopyFromData copy_from_data;
	FtkBitmapCopyToData   copy_to_data;
}PrivInfo;

static Ret ftk_display_sigma_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	DECL_PRIV(thiz, priv);
	int display_width  = priv->width;
	int display_height = priv->height;

	ftk_logd("%s: ox=%d oy=%d (%d %d %d %d)\n", __func__, xoffset, yoffset, 
		rect->x, rect->y, rect->width, rect->height);
	return priv->copy_to_data(bitmap, rect, 
		priv->bits, xoffset, yoffset, display_width, display_height); 
}

static int ftk_display_sigma_width(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);

	return priv->width;
}

static int ftk_display_sigma_height(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);

	return priv->width;
}

static Ret ftk_display_sigma_snap(FtkDisplay* thiz, FtkRect* r, FtkBitmap* bitmap)
{
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	int w = ftk_display_width(thiz);
	int h = ftk_display_height(thiz);
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	
	rect.x = r->x;
	rect.y = r->y;
	rect.width = FTK_MIN(bw, r->width);
	rect.height = FTK_MIN(bh, r->height);

	return priv->copy_from_data(bitmap, priv->bits, w, h, &rect);
}

static void ftk_display_sigma_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		FTK_ZFREE(thiz, sizeof(FtkDisplay) + sizeof(PrivInfo));
	}

	return;
}

FtkDisplay* ftk_display_sigma_create(void* osd_addr, int width, int height, int bits_per_pixel, void* osd)
{
	FtkDisplay* thiz = NULL;
	return_val_if_fail(osd_addr != NULL, NULL);

	thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->update   = ftk_display_sigma_update;
		thiz->width    = ftk_display_sigma_width;
		thiz->height   = ftk_display_sigma_height;
		thiz->snap     = ftk_display_sigma_snap;
		thiz->destroy  = ftk_display_sigma_destroy;

		priv->width  = width;
		priv->height = height;
		priv->bits   = osd_addr;
		priv->p_osd  = osd;
		priv->bits_per_pixel = bits_per_pixel;

		if(bits_per_pixel == 16)
		{
			priv->copy_to_data   = ftk_bitmap_copy_to_data_rgb565;
			priv->copy_from_data = ftk_bitmap_copy_from_data_rgb565;
		}
		else if(bits_per_pixel == 24)
		{
			priv->copy_to_data   = ftk_bitmap_copy_to_data_bgr24;
			priv->copy_from_data = ftk_bitmap_copy_from_data_bgr24;
		}
		else if(bits_per_pixel == 32)
		{
			priv->copy_to_data   = ftk_bitmap_copy_to_data_bgra32;
			priv->copy_from_data = ftk_bitmap_copy_from_data_bgra32;
		}
		else
		{
			assert(!"not supported framebuffer format.");
		}
	}
		
	return thiz;
}

