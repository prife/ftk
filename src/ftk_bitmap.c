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

#include "ftk_log.h"
#include "ftk_bitmap.h"

struct _FtkBitmap
{
	size_t w;
	size_t h;
	int ref;
	size_t length;
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

size_t ftk_bitmap_width(FtkBitmap* thiz)
{
	return thiz != NULL ? thiz->w : 0;
}

size_t ftk_bitmap_height(FtkBitmap* thiz)
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

void       ftk_bitmap_clear(FtkBitmap* thiz, FtkColor c)
{
	int i = 0;
	int j = 0;
	FtkColor* p = NULL;

	return_if_fail(thiz != NULL);

	p = thiz->bits;
	for(i = 0; i < thiz->h; i++)
	{
		for(j = 0; j < thiz->w; j++, p++)
		{
			*p = c;
		}
	}
}

Ret        ftk_bitmap_copy_from_bitmap(FtkBitmap* thiz, FtkBitmap* other)
{
	size_t width = 0;
	size_t height = 0;
	FtkColor* dst = ftk_bitmap_bits(thiz);
	FtkColor* src = ftk_bitmap_bits(other);
	return_val_if_fail(dst != NULL && src != NULL, RET_FAIL);

	width = FTK_MIN(thiz->w, other->w);
	height = FTK_MIN(thiz->h, other->h);

	for(; height; height--)
	{
		memcpy(dst, src, sizeof(FtkColor) * width);
		dst += thiz->w;
		src += other->w;
	}

	return RET_OK;
}

#define COPY_FROM_DECL(Type) \
	int x  = 0;\
	int y  = 0;\
	int w  = 0;\
	int h  = 0;\
	int ox = 0;\
	int oy = 0;\
	int bw = ftk_bitmap_width(bitmap);\
	int bh = ftk_bitmap_height(bitmap);\
	Type* src = data;\
	FtkColor* dst = ftk_bitmap_bits(bitmap);\
	\
	return_val_if_fail(src != NULL && dst != NULL, RET_FAIL);\
	\
	x = rect != NULL ? rect->x : 0;\
	y = rect != NULL ? rect->y : 0;\
	x = x < 0 ? 0 : x;\
	y = y < 0 ? 0 : y;\
	return_val_if_fail(x < dw && y < dh, RET_FAIL);\
	\
	w = rect != NULL ? rect->width  : bw;\
	h = rect != NULL ? rect->height : bh;\
	\
	/*width/height must less than bitmap's width/height*/\
	w = w < bw ? w : bw;\
	h = w < bh ? h : bh;\
	\
	/*width/height must less than data's width/height*/\
	w = (x + w) < dw ? w : dw - x;\
	h = (y + h) < dh ? h : dh - y;

#define COPY_TO_DECL(Type) \
	int i = 0;\
	int j = 0;\
	int k = 0;\
	int bw = ftk_bitmap_width(bitmap);\
	int bh = ftk_bitmap_height(bitmap);\
	int x = rect != NULL ? rect->x : 0;\
	int y = rect != NULL ? rect->y : 0;\
	int w = rect != NULL ? rect->width : bw;\
	int h = rect != NULL ? rect->height : bh;\
	Type* dst = data;\
	FtkColor* src = ftk_bitmap_bits(bitmap);\
	\
	return_val_if_fail(ox < dw, RET_FAIL);\
	return_val_if_fail(oy < dh, RET_FAIL);\
	return_val_if_fail(x < bw, RET_FAIL);\
	return_val_if_fail(y < bh, RET_FAIL);\
	return_val_if_fail(dst != NULL && src != NULL, RET_FAIL);\
	\
	x = x < 0 ? 0 : x;\
	y = y < 0 ? 0 : y;\
	\
	w = (x + w) < bw ? w : bw - x;\
	h = (y + h) < bh ? h : bh - y;\
	w = (ox + w) < dw ? w : dw - ox;\
	h = (oy + h) < dh ? h : dh - oy;\
	\
	w += x;\
	h += y;

/*=====================================================================*/
Ret ftk_bitmap_copy_from_data_bgr24(FtkBitmap* bitmap, void* data, 
	size_t dw, size_t dh, FtkRect* rect)
{
	COPY_FROM_DECL(unsigned char);
	
	src += 3*(y * dw + x);
	for(oy = 0; oy < h; oy++)
	{
		unsigned char* psrc = src;
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
	unsigned char alpha = 0;
	COPY_TO_DECL(unsigned char);
	(void)k;	

	src += y * bw;
	dst += 3*(oy * dw);
	for(i = y; i < h; i++)
	{
		unsigned char* pdst = dst + 3*ox;
		for(j = x; j < w; j++, pdst+=3)
		{
			FtkColor* psrc = src+j;
			if(likely(psrc->a == 0xff))
			{
				pdst[0] = psrc->b;
				pdst[1] = psrc->g;
				pdst[2] = psrc->r;
			}
			else
			{
				alpha = psrc->a;
				FTK_ALPHA_1(psrc->b, pdst[0], alpha);
				FTK_ALPHA_1(psrc->g, pdst[1], alpha);
				FTK_ALPHA_1(psrc->r, pdst[2], alpha);
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
	COPY_FROM_DECL(FtkColor);

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
	unsigned char alpha = 0;
	COPY_TO_DECL(FtkColor);

	src += y * bw;
	dst += oy * dw;
	for(i = y; i < h; i++)
	{
		for(j = x, k = ox; j < w; j++, k++)
		{
			FtkColor* pdst = dst+k;
			FtkColor* psrc = src+j;

			if(likely(psrc->a == 0xff))
			{
				*(unsigned int*)(pdst) = *(unsigned int*)(psrc);	
			}
			else
			{
				alpha = psrc->a;
				FTK_ALPHA_1(psrc->b, pdst->b, alpha);
				FTK_ALPHA_1(psrc->g, pdst->g, alpha);
				FTK_ALPHA_1(psrc->r, pdst->r, alpha);
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
	COPY_FROM_DECL(unsigned short);
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
	unsigned int color;
	FtkColor* psrc = NULL;
	FtkColor dcolor = {0};
	FtkColor* pcolor = NULL;
	unsigned char alpha = 0;
	unsigned short pixel = 0;
	unsigned short* pdst = NULL;
	COPY_TO_DECL(unsigned short);
	src += y * bw + x;
	dst += oy * dw + ox;

	pcolor = &dcolor;
	for(i = h - y; i; i--)
	{
		psrc = src;
		pdst = dst;
		
		for(j = w; j; j--, psrc++, pdst++)
		{
			if(likely(psrc->a == 0xff))
			{
				/*FIXME: works on little end only.*/
				color = *(unsigned int*)psrc;
				*pdst = (color & 0xf80000) >> 8 | ((color & 0xfc00) >> 5) | ((color & 0xf8) >> 3);
			}
			else
			{
				pixel = *pdst;
				alpha = psrc->a;
				dcolor.r = (pixel & 0xf800) >> 8;
				dcolor.g = (pixel & 0x07e0) >> 3;
				dcolor.b = (pixel & 0x1f) << 3;
				FTK_ALPHA(psrc, pcolor, alpha);
				pixel = ((dcolor.r >> 3) << 11) | ((dcolor.g >> 2) << 5) | (dcolor.b >> 3);
				*pdst = pixel;
			}
		}
		src += bw;
		dst += dw;
	}

	return RET_OK;
}

/*=====================================================================*/

Ret ftk_bitmap_copy_from_data_argb32(FtkBitmap* bitmap, void* data, 
	size_t dw, size_t dh, FtkRect* rect)
{
	COPY_FROM_DECL(unsigned char);
	
	src += y * dw + x;
	for(oy = 0; oy < h; oy++)
	{
		for(ox = 0; ox < w; ox++)
		{
			unsigned char* psrc =(unsigned char*) (src + ox);
			dst[ox].r = psrc[1];
			dst[ox].g = psrc[2];
			dst[ox].b = psrc[3]; 
			dst[ox].a = psrc[0];
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_argb32(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
{
	COPY_TO_DECL(FtkColor);
	ftk_logd("%s:%d\n", __func__, __LINE__);
	
	src += y * bw;
	dst += oy * dw;
	for(i = y; i < h; i++)
	{
		for(j = x, k = ox; j < w; j++, k++)
		{
			unsigned char* pdst = (unsigned char*)(dst+k);
			FtkColor* psrc = src+j;

			if(likely(psrc->a == 0xff))
			{
				pdst[3] = psrc->b;
				pdst[2] = psrc->g;
				pdst[1] = psrc->r;
				pdst[0] = 0xff;
			}
			else
			{
				FTK_ALPHA_1(psrc->r, pdst[1], psrc->a);
				FTK_ALPHA_1(psrc->g, pdst[2], psrc->a);
				FTK_ALPHA_1(psrc->b, pdst[3], psrc->a);
				pdst[0] = psrc->a;
			}
			if(i == y && j == x)
			{
				ftk_logd("%s: dst(%02x %02x %02x %02x)\n", __func__, 
					pdst[0], pdst[1], pdst[2], pdst[3]);
				ftk_logd("%s: color(%02x %02x %02x %02x)\n", __func__,
					psrc->a, psrc->r, psrc->g, psrc->b);
			}
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
	COPY_FROM_DECL(unsigned char);

	src += y * dw + x;
	for(oy = 0; oy < h; oy++)
	{
		for(ox = 0; ox < w; ox++)
		{
			FtkColor* pdst = dst + ox;
			unsigned char* psrc =(unsigned char*) (src + ox);
			pdst->r = psrc[0];
			pdst->g = psrc[1];
			pdst->b = psrc[2]; 
			pdst->a = psrc[3];
		}
		src += dw; 
		dst += bw;
	}

	return RET_OK;
}

Ret ftk_bitmap_copy_to_data_rgba32(FtkBitmap* bitmap, FtkRect* rect, void* data, int ox, int oy, size_t dw, size_t dh)
{
	COPY_TO_DECL(FtkColor);
	
	src += y * bw;
	dst += oy * dw;
	for(i = y; i < h; i++)
	{
		for(j = x, k = ox; j < w; j++, k++)
		{
			unsigned char* pdst = (unsigned char*)(dst+k);
			FtkColor* psrc = src+j;

			if(likely(psrc->a == 0xff))
			{
				pdst[0] = psrc->r;
				pdst[1] = psrc->g;
				pdst[2] = psrc->b;
				pdst[3] = 0xff;
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

