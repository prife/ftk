/*
 * File: ftk_display_fb.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include "ftk_display_fb.h"
#include "ftk_log.h"

struct FB 
{
	int fd;
	unsigned size;
	unsigned short *bits;
	struct fb_fix_screeninfo fi;
	struct fb_var_screeninfo vi;
};

#define fb_width(fb) ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_size(fb) ((fb)->vi.xres * (fb)->vi.yres * 2)

static int fb_open(struct FB *fb, const char* fbfilename)
{
	fb->fd = open(fbfilename, O_RDWR);
	if (fb->fd < 0)
	{
		fprintf(stderr, "can't open %s\n", fbfilename);
		return -1;
	}

	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
		goto fail;
	if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
		goto fail;

	fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE, 
					MAP_SHARED, fb->fd, 0);
	if (fb->bits == MAP_FAILED)
		goto fail;

	return 0;
fail:
	printf("%s is not a framebuffer.\n", fbfilename);
	close(fb->fd);

	return -1;
}

static void fb_close(struct FB *fb)
{
	munmap(fb->bits, fb_size(fb));
	close(fb->fd);
}

typedef struct _PrivInfo
{
	struct FB fb;
}PrivInfo;

static Ret ftk_display_fb_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int x = rect->x;
	int y = rect->y;
	int w = rect->width;
	int h = rect->height;
	DECL_PRIV(thiz, priv);
	int display_width  = fb_width(&priv->fb);
	int display_height = fb_height(&priv->fb);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	ftk_logv("%s: ox=%d oy=%d x=%d y=%d w=%d h=%d\n", __func__, xoffset, yoffset, x, y, w, h);
	return_val_if_fail(bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < bitmap_width, RET_FAIL);
	return_val_if_fail(y < bitmap_height, RET_FAIL);
	return_val_if_fail(xoffset < display_width, RET_FAIL);
	return_val_if_fail(yoffset < display_height, RET_FAIL);

	FtkColor dcolor;
	FtkColor* pdst = NULL;
	FtkColor* psrc = NULL;
	unsigned short pixel = 0;
	FtkColor* src = ftk_bitmap_bits(bitmap);
	unsigned short* dst = priv->fb.bits;
	w = (x + w) < bitmap_width  ? w : bitmap_width - x;
	w = (xoffset + w) < display_width  ? w : display_width  - xoffset;
	h = (y + h) < bitmap_height ? h : bitmap_height - y;
	h = (yoffset + h) < display_height ? h : display_height - yoffset;
	
	w += x;
	h += y;

	src += y * bitmap_width;
	dst += yoffset * display_width;

	for(i = y; i < h; i++)
	{
		for(j = x, k = xoffset; j < w; j++, k++)
		{
			dcolor.r = (dst[k] & 0xf800) >> 8;
			dcolor.g = (dst[k] & 0x07e0) >> 3;
			dcolor.b = (dst[k] & 0x1f) << 3;
			psrc = src + j;
			pdst = &dcolor;

			FTK_ALPHA(psrc, pdst, psrc->a);

			pixel = ((dcolor.r >> 3) << 11) | ((dcolor.g >> 2) << 5) | (dcolor.b >> 3);
			dst[k] = pixel;
		}
		src += bitmap_width;
		dst += display_width;
	}

	return RET_OK;
}

static int ftk_display_fb_width(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);

	return priv->fb.vi.xres;
}

static int ftk_display_fb_height(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);

	return priv->fb.vi.yres;
}

static int ftk_display_fb_bits_per_pixel(FtkDisplay* thiz)
{
//	DECL_PRIV(thiz, priv);

	return 2;
}

static void ftk_display_fb_destroy(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	fb_close(&priv->fb);
	free(thiz);

	return;
}

FtkDisplay* ftk_display_fb_create(const char* filename)
{
	FtkDisplay* thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		if(fb_open(&priv->fb, filename) == 0)
		{
			thiz->update   = ftk_display_fb_update;
			thiz->width    = ftk_display_fb_width;
			thiz->height   = ftk_display_fb_height;
			thiz->bits_per_pixel = ftk_display_fb_bits_per_pixel;
			thiz->destroy = ftk_display_fb_destroy;
		}
		else
		{
			free(thiz);
			thiz = NULL;
		}
	}
		
	return thiz;
}

