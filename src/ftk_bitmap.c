/*
 * File: ftk_bitmap.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   Ftk bitmap
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

/*=====================================================================*/
Ret ftk_bitmap_copy_from_data_bgr24(FtkBitmap* bitmap, void* data, 
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
	unsigned char* src = data;
	unsigned char* psrc = NULL;
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

	src += 3*(y * dw + x);
	for(oy = 0; oy < h; oy++)
	{
		psrc = src;
		for(ox = 0; ox < w; ox++, psrc+=3)
		{
			dst[ox].a = 0xff;
			dst[ox].b = psrc[0];
			dst[ox].g = psrc[1];
			dst[ox].r = psrc[2];
		}
		src += 3*dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_bgr24(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
{
	int i = 0;
	int j = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int x = rect != NULL ? rect->x : 0;
	int y = rect != NULL ? rect->y : 0;
	int w = rect != NULL ? rect->width : bw;
	int h = rect != NULL ? rect->height : bh;
	unsigned char* dst = data;
	unsigned char* pdst = NULL;
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
	dst += 3*(oy * dw);

	for(i = y; i < h; i++)
	{
		pdst = dst + 3*ox;
		for(j = x; j < w; j++, pdst+=3)
		{
			FtkColor* psrc = src+j;
			if(psrc->a == 0xff)
			{
				pdst[0] = psrc->b;
				pdst[1] = psrc->g;
				pdst[2] = psrc->r;
			}
			else
			{
				FTK_ALPHA_1(psrc->b, pdst[0], psrc->a);
				FTK_ALPHA_1(psrc->g, pdst[1], psrc->a);
				FTK_ALPHA_1(psrc->r, pdst[2], psrc->a);
			}
		}
		src += bw;
		dst += 3*dw;
	}

	return RET_OK;
}

/*=====================================================================*/
Ret ftk_bitmap_copy_from_data_bgra32(FtkBitmap* bitmap, void* data, 
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
			dst[ox] = src[ox];
			dst[ox].a = 0xff;
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_bgra32(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
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
				*pdst = *psrc;
			}
			else
			{
				FTK_ALPHA_1(psrc->b, pdst->b, psrc->a);
				FTK_ALPHA_1(psrc->g, pdst->g, psrc->a);
				FTK_ALPHA_1(psrc->r, pdst->r, psrc->a);
			}
		}
		src += bw;
		dst += dw;
	}

	return RET_OK;
}
/*=====================================================================*/

Ret ftk_bitmap_copy_from_data_rgb565(FtkBitmap* bitmap, void* data, 
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
	unsigned short* src = data;
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
			dst[ox].r = (src[ox] >> 8) & 0xf8;
			dst[ox].g = (src[ox] >> 3) & 0xfc;
			dst[ox].b = (src[ox] << 3) & 0xff;
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_rgb565(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
{
	int i = 0;
	int j = 0;
	int k = 0;
	FtkColor dcolor;
	FtkColor* pdst = NULL;
	FtkColor* psrc = NULL;
	unsigned short pixel = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int x = rect != NULL ? rect->x : 0;
	int y = rect != NULL ? rect->y : 0;
	int w = rect != NULL ? rect->width : bw;
	int h = rect != NULL ? rect->height : bh;
	unsigned short* dst = data;
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
			dcolor.r = (dst[k] & 0xf800) >> 8;
			dcolor.g = (dst[k] & 0x07e0) >> 3;
			dcolor.b = (dst[k] & 0x1f) << 3;
			psrc = src + j;
			pdst = &dcolor;

			if(psrc->a == 0xff)
			{
				*pdst = *psrc;
			}
			else
			{
				FTK_ALPHA(psrc, pdst, psrc->a);
			}
			pixel = ((dcolor.r >> 3) << 11) | ((dcolor.g >> 2) << 5) | (dcolor.b >> 3);
			dst[k] = pixel;
		}
		src += bw;
		dst += dw;
	}

	return RET_OK;
}

/*=====================================================================*/
Ret ftk_bitmap_copy_from_data_rgba32(FtkBitmap* bitmap, void* data, 
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
	FtkColor* src = (FtkColor*)data;	/* data is RGBA */
	unsigned char* psrc = NULL;
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
			psrc =(unsigned char*) (src + ox);
			dst[ox].r = psrc[0];
			dst[ox].g = psrc[1];
			dst[ox].b = psrc[2]; 
			dst[ox].a = psrc[3];
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_rgba32(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
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
	FtkColor* dst = data;	/* data is RGBA */
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
			unsigned char* pdst = (unsigned char*)(dst+k);
			FtkColor* psrc = src+j;
			if(psrc->a == 0xff)
			{
				pdst[0] = psrc->r;
				pdst[1] = psrc->g;
				pdst[2] = psrc->b;
				pdst[3] = psrc->a;
			}
			else
			{
				FTK_ALPHA_1(psrc->r, pdst[0], psrc->a);
				FTK_ALPHA_1(psrc->g, pdst[1], psrc->a);
				FTK_ALPHA_1(psrc->b, pdst[2], psrc->a);
				pdst[3] = psrc->a;
			}
		}
		src += bw;
		dst += dw;
	}

	return RET_OK;
}
/*=====================================================================*/
