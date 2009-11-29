/*
 * File: ftk_bitmap.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   Ftk bitmap
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

#include "ftk_bitmap.h"

struct _FtkBitmap
{
	int w;
	int h;
	int ref;
	int length;
	FtkColor bits[1];
};

FtkBitmap* ftk_bitmap_create(int w, int h, FtkColor color)
{
	int length = w * h;
	FtkBitmap* thiz = (FtkBitmap*)FTK_ALLOC(sizeof(FtkBitmap) + length * sizeof(FtkColor));

	if(thiz != NULL)
	{
		int i = 0;
		thiz->w = w;
		thiz->h = h;
		thiz->ref = 1;
		thiz->length = length;
		
		for(i = 0; i < length; i++) 
		{
			thiz->bits[i] = color;
		}
	}

	return thiz;
}

int       ftk_bitmap_width(FtkBitmap* thiz)
{
	return thiz != NULL ? thiz->w : 0;
}

int       ftk_bitmap_height(FtkBitmap* thiz)
{
	return thiz != NULL ? thiz->h : 0;
}

FtkColor* ftk_bitmap_bits(FtkBitmap* thiz)
{
	return thiz != NULL ? thiz->bits : NULL;
}

static void ftk_bitmap_destroy(FtkBitmap* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkBitmap) + thiz->length * sizeof(FtkColor));
	}

	return;
}

void       ftk_bitmap_ref(FtkBitmap* thiz)
{
	if(thiz != NULL)
	{
		thiz->ref++;
	}

	return;
}

void       ftk_bitmap_unref(FtkBitmap* thiz)
{
	if(thiz != NULL)
	{
		thiz->ref--;
		if(thiz->ref == 0)
		{
			ftk_bitmap_destroy(thiz);
		}
	}

	return;
}

Ret ftk_bitmap_copy_from_data_argb(FtkBitmap* bitmap, void* data, 
	size_t dw, size_t dh, FtkRect* rect)
{
	int x  = 0;
	int y  = 0;
	int w  = 0;
	int h  = 0;
	int ox = 0;
	int oy = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	FtkColor* src = (FtkColor*)data;
	FtkColor* dst = ftk_bitmap_bits(bitmap);

	return_val_if_fail(src != NULL && dst != NULL, RET_FAIL);
	
	x = rect != NULL ? rect->x : 0;
	y = rect != NULL ? rect->y : 0;
	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	return_val_if_fail(x < dw && y < dh, RET_FAIL);

	w = rect != NULL ? rect->width  : bw;
	h = rect != NULL ? rect->height : bh;

	/*width/height must less than bitmap's width/height*/
	w = w < bw ? w : bw;
	h = w < bh ? h : bh;
	
	/*width/height must less than data's width/height*/
	w = (x + w) < dw ? w : dw - x;
	h = (y + h) < dh ? h : dh - y;

	src += y * dw + x;
	for(oy = 0; oy < h; oy++)
	{
		for(ox = 0; ox < w; ox++)
		{
			dst[ox].a = 0xff;
			dst[ox].r = src[ox].b;
			dst[ox].g = src[ox].g;
			dst[ox].b = src[ox].r;
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_argb(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int x = rect != NULL ? rect->x : 0;
	int y = rect != NULL ? rect->y : 0;
	int w = rect != NULL ? rect->width : bw;
	int h = rect != NULL ? rect->height : bh;
	FtkColor* dst = data;
	FtkColor* src = ftk_bitmap_bits(bitmap);

	return_val_if_fail(ox < dw, RET_FAIL);
	return_val_if_fail(oy < dh, RET_FAIL);
	return_val_if_fail(x < bw, RET_FAIL);
	return_val_if_fail(y < bh, RET_FAIL);
	return_val_if_fail(dst != NULL && src != NULL, RET_FAIL);

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	
	w = (x + w) < bw ? w : bw - x;
	h = (y + h) < bh ? h : bh - y;
	w = (ox + w) < dw ? w : dw - ox;
	h = (oy + h) < dh ? h : dh - oy;
	
	w += x;
	h += y;

	src += y * bw;
	dst += oy * dw;

	for(i = y; i < h; i++)
	{
		for(j = x, k = ox; j < w; j++, k++)
		{
			FtkColor* pdst = dst+k;
			FtkColor* psrc = src+j;
			if(psrc->a == 0xff)
			{
				pdst->b = psrc->r;
				pdst->g = psrc->g;
				pdst->r = psrc->b;
			}
			else
			{
				FTK_ALPHA_1(psrc->r, pdst->b, psrc->a);
				FTK_ALPHA_1(psrc->b, pdst->r, psrc->a);
				FTK_ALPHA_1(psrc->g, pdst->g, psrc->a);
			}
		}
		src += bw;
		dst += dw;
	}

	return RET_OK;
}

