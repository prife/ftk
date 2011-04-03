/*
 * File: ftk_canvas.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   
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
 * 2010-3-16 woodysu<yapo_su@hotmail.com> modified. 
 */
#include "ftk_log.h"
#include "ftk_util.h"
#include "ftk_bitmap.h"
#include "ftk_canvas.h"

struct _FtkCanvas
{
	FtkGc gc;
	size_t width;
	size_t height;
	FtkColor* bits;
	FtkBitmap* bitmap;
};

FtkCanvas* ftk_canvas_create(int w, int h, FtkColor clear_color)
{
	FtkCanvas* thiz = (FtkCanvas*)FTK_ZALLOC(sizeof(FtkCanvas));
	if(thiz != NULL)
	{
		thiz->gc.bg = clear_color;
		thiz->gc.fg.a = 0xff;
		thiz->gc.fg.r = 0xff - clear_color.r;
		thiz->gc.fg.g = 0xff - clear_color.g;
		thiz->gc.fg.b = 0xff - clear_color.b;

		thiz->bitmap = ftk_bitmap_create(w, h, clear_color);
		thiz->bits = ftk_bitmap_bits(thiz->bitmap);
		thiz->width = ftk_bitmap_width(thiz->bitmap);
		thiz->height = ftk_bitmap_height(thiz->bitmap);
	}

	return thiz;
}

Ret ftk_canvas_reset_gc(FtkCanvas* thiz, FtkGc* gc)
{
	return_val_if_fail(thiz != NULL && gc != NULL, RET_FAIL);

	thiz->gc.mask = 0;

	return ftk_gc_copy(&thiz->gc, gc);
}

Ret ftk_canvas_set_gc(FtkCanvas* thiz, FtkGc* gc)
{
	return_val_if_fail(thiz != NULL && gc != NULL, RET_FAIL);

	return ftk_gc_copy(&thiz->gc, gc);
}

FtkGc* ftk_canvas_get_gc(FtkCanvas* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return &thiz->gc;
}

#define PUT_PIXEL(pdst, color, alpha) \
	do\
	{\
		if(likely(alpha == 0xff))\
		{\
			pdst[0] = *color;\
		}\
		else\
		{\
			FTK_ALPHA(color, pdst, alpha);\
		}\
	}while(0);

Ret ftk_canvas_put_pixel(FtkCanvas* thiz, int x, int y, FtkColor val)
{
	int width = 0;
	int height = 0;
	FtkColor* bits = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(bits != NULL && x < width && y < height, RET_FAIL);
	bits[y * width + x] = val;

	return RET_OK;
}

FtkColor* ftk_canvas_get_pixel(FtkCanvas* thiz, int x, int y)
{
	int width = 0;
	int height = 0;
	FtkColor* bits = NULL;
	return_val_if_fail(thiz != NULL, NULL);

	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(bits != NULL && x < width && y < height, NULL);

	return bits + y * width + x;
}

Ret ftk_canvas_draw_vline(FtkCanvas* thiz, int x, int y, int h)
{
	int i = 0;
	int width = 0;
	int height = 0;
	FtkColor* bits = NULL;
	unsigned char alpha = 0;
	FtkColor* pdst = NULL;
	FtkColor* color = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	
	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(bits != NULL && y < height, RET_FAIL);
	alpha = thiz->gc.mask & FTK_GC_ALPHA ? thiz->gc.alpha :  thiz->gc.fg.a;

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	h = (y + h) < height ? h : (height - y);

	pdst = bits + width * y + x;
	color = &(thiz->gc.fg);
	if(thiz->gc.mask & FTK_GC_LINE_MASK)
	{
		unsigned int line_mask = thiz->gc.line_mask;
		for(i = 0; i < h; i++, pdst +=width)
		{
			if(FTK_MASK_BITS(line_mask, i))
			{
				PUT_PIXEL(pdst, color, alpha);
			}
		}
	}
	else
	{
		for(i = h; i; i--, pdst+=width)
		{
			PUT_PIXEL(pdst, color, alpha);
		}
	}

	return RET_OK;
}

Ret ftk_canvas_draw_hline(FtkCanvas* thiz, int x, int y, int w)
{
	int i = 0;
	int width = 0;
	int height = 0;
	FtkColor* bits = NULL;
	unsigned char alpha = 0;
	FtkColor* pdst = NULL;
	FtkColor* color = NULL;	
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(bits != NULL && x < width, RET_FAIL);
	return_val_if_fail(y < height, RET_FAIL);	
	alpha = thiz->gc.mask & FTK_GC_ALPHA ? thiz->gc.alpha :  thiz->gc.fg.a;

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	w = (x + w) < width ? w : width - x;
	color = &(thiz->gc.fg);
	pdst = bits + y * width + x;
	if(thiz->gc.mask & FTK_GC_LINE_MASK)
	{
		unsigned int line_mask = thiz->gc.line_mask;
		for(i = 0; i < w; i++, pdst++)
		{
			if(FTK_MASK_BITS(line_mask, i))
			{
				PUT_PIXEL(pdst, color, alpha);
			}
		}
	}
	else
	{
		for(i = w; i; i--, pdst++)
		{
			PUT_PIXEL(pdst, color, alpha);
		}
	}

	return RET_OK;
}

Ret ftk_canvas_fast_fill_rect(FtkCanvas* thiz, int x, int y, int w, int h)
{
	int width = 0;
	int height = 0;
	int iter_w = 0;
	int iter_h = 0;
	FtkColor color;
	FtkColor* bits = NULL;
	unsigned char alpha = 0;
	FtkColor* pdst = NULL;
	
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(bits != NULL && x < width, RET_FAIL);
	return_val_if_fail(y < height, RET_FAIL);	
	alpha = thiz->gc.mask & FTK_GC_ALPHA ? thiz->gc.alpha :  thiz->gc.fg.a;

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	w = (x + w) < width ? w : width - x;
	h = (y + h) < height ? h : height - y;

	iter_w = w;
	iter_h = h;
	bits += y * width;

	color = thiz->gc.fg;
	while(iter_h--)
	{
		pdst = bits + x;
		while(iter_w--)
		{
			*pdst = color;
			pdst++;
		}
		iter_w = w;
		bits += width;
	}

	return RET_OK;
}

Ret ftk_canvas_draw_rect(FtkCanvas* thiz, int x, int y, int w, int h, int fill)
{
	int i = 0;
	int width  = 0;
	int height = 0;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = thiz->width;
	height = thiz->height;
	return_val_if_fail(x < width && y < height, RET_FAIL);

	if(fill)
	{
		for(i = 0; i < h; i++)
		{
			if((y + i) < height)
			{
				ftk_canvas_draw_hline(thiz, x, (y+i), w);	
			}
		}
	}
	else
	{
		ftk_canvas_draw_hline(thiz, x,   y, w);		
		ftk_canvas_draw_hline(thiz, x,   y+h-1, w);		
		ftk_canvas_draw_vline(thiz, x,   y, h);		
		ftk_canvas_draw_vline(thiz, x+w-1, y, h);		
	}

	return RET_OK;
}

Ret ftk_canvas_draw_round_rect(FtkCanvas* thiz, int x, int y, int w, int h, int fill)
{
	int width  = 0;
	int height = 0;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = thiz->width;
	height = thiz->height;
	return_val_if_fail(x < width && y < height, RET_FAIL);
	return_val_if_fail(w > 8 && h > 8, RET_FAIL);

	ftk_canvas_draw_hline(thiz, x + 2, y, w-4);
	ftk_canvas_draw_hline(thiz, x + 1, y + 1, w-2);
	ftk_canvas_draw_vline(thiz, x, y + 2, h - 4);
	ftk_canvas_draw_vline(thiz, x + 1, y + 1, h - 2);

	ftk_canvas_draw_vline(thiz, x+w-1, y + 2, h - 4);
	ftk_canvas_draw_vline(thiz, x + w -2, y + 1, h - 2);
	ftk_canvas_draw_hline(thiz, x + 1, y + h - 1, w-2);
	ftk_canvas_draw_hline(thiz, x + 2, y + h - 2, w-4);

	if(fill)
	{
		ftk_canvas_draw_rect(thiz, x + 2, y + 2, w - 4, h - 4, fill);
	}

	return RET_OK;
}

Ret ftk_canvas_draw_string_ex(FtkCanvas* thiz, int x, int y, const char* str, int len, int vcenter)
{
	int i = 0;
	int j = 0;
	int ox = x;
	int oy = y;
	int width = 0;
	int height = 0;
	int vcenter_offset = 0;
	FtkGlyph glyph = {0};
	FtkColor color = {0};
	FtkColor fg = {0};
	FtkColor bg = {0};
	FtkColor* bits = NULL;
	unsigned short code = 0;
	const char* iter = str;
	return_val_if_fail(thiz != NULL && str != NULL, RET_FAIL);

	len = len >= 0 ? len : (int)strlen(str);
	width  = thiz->width;
	height = thiz->height;
	bits   = thiz->bits;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	color.a = 0xff;
	fg = thiz->gc.fg;

	/*FIXME: vcenter_offset maybe not correct.*/
	vcenter_offset = ftk_font_height(thiz->gc.font)/3;
	while(*iter && (iter - str) < len)
	{
		int offset = 0;
		code = utf8_get_char(iter, &iter);

		if(code == ' ' )
		{
			x += FTK_SPACE_WIDTH;
			ox = x;
			continue;
		}
		else if(code == '\t')
		{
			x += FTK_TAB_WIDTH * FTK_SPACE_WIDTH;
			ox = x;
			continue;
		}

		if(code == 0xffff || code == 0) break;
		if(code == '\r' || code == '\n' || ftk_font_lookup(thiz->gc.font, code, &glyph) != RET_OK) 
			continue;

		glyph.y = vcenter ? glyph.y - vcenter_offset : glyph.y;
		if((x + glyph.x + glyph.w) >= width) break;
		if((y - glyph.y + glyph.h) >= height) break;

		x = x + glyph.x;
		y = y - glyph.y;
		for(i = 0; i < glyph.h; i++,y++)
		{
			for(j = 0, x= ox; j < glyph.w; j++,x++)
			{
				unsigned char data = glyph.data[i * glyph.w + j];
				offset = y * width + x;
				bg = bits[offset];
				if(data)
				{
					color.r = FTK_ALPHA_1(fg.r, bg.r, data);
					color.g = FTK_ALPHA_1(fg.g, bg.g, data);
					color.b = FTK_ALPHA_1(fg.b, bg.b, data);
					bits[offset] = color;
				}
			}
		}

		y = oy;
		x = ox + glyph.x + glyph.w + 1;
		ox = x;
	}

	return RET_OK;
}

Ret ftk_canvas_draw_string(FtkCanvas* thiz, int x, int y, const char* str, int len)
{
	return ftk_canvas_draw_string_ex(thiz, x, y, str, len, 0);
}

/* add by woodysu*/
/* the bitmap is Src , the FtkCanvas.bitmap is Dst*/
Ret ftk_canvas_draw_bitmap_zoom(FtkCanvas* thiz, FtkBitmap* bitmap, int x, int y, int w, int h, unsigned char alpha)
{
	int i=0, j=0, m=0, n=0;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;
	FtkColor* psrc = NULL;
	FtkColor* pdst = NULL;
	int width  = thiz->width; //Canvas width
	int height = thiz->height;
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < width, RET_FAIL);
	return_val_if_fail(y < height, RET_FAIL);

	src = ftk_bitmap_bits(bitmap);
	dst = thiz->bits;
        
	if(w+x>width)
		return RET_FAIL;

	if(h+y>height)
		return RET_FAIL;
	
	for(j=0 ; j<h ; j++)
	{
		n=j*bitmap_height/h;
		for(i=0 ; i<w ; i++)
		{
			m=i*bitmap_width/w;
			pdst = dst + (j+y)*width+x+i;
			psrc = src + n*bitmap_width+m;

			if(psrc->a == 0xff)
			{
				*pdst = *psrc;
			}	
			else
			{
				FTK_ALPHA(psrc, pdst, psrc->a);
			}
		}	
	}
	
	return 	RET_OK;

}

Ret ftk_canvas_draw_bitmap(FtkCanvas* thiz, FtkBitmap* bitmap, int x, int y, int w, int h, int xoffset, int yoffset)
{
	int i = 0;
	int j = 0;
	int k = 0;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;
	FtkColor* psrc = NULL;
	FtkColor* pdst = NULL;
	unsigned char alpha = 0;
	int width  = thiz->width;
	int height = thiz->height;
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < bitmap_width, RET_FAIL);
	return_val_if_fail(y < bitmap_height, RET_FAIL);
	return_val_if_fail(xoffset < width, RET_FAIL);
	return_val_if_fail(yoffset < height, RET_FAIL);

	src = ftk_bitmap_bits(bitmap);
	dst = thiz->bits;

	w = (x + w) < bitmap_width  ? w : bitmap_width - x;
	w = (xoffset + w) < width  ? w : width  - xoffset;
	h = (y + h) < bitmap_height ? h : bitmap_height - y;
	h = (yoffset + h) < height ? h : height - yoffset;
	
	w += x;
	h += y;

	src += y * bitmap_width;
	dst += yoffset * width;

	if(thiz->gc.mask & FTK_GC_ALPHA)
	{
		for(i = y; i < h; i++)
		{
			for(j = x, k = xoffset; j < w; j++, k++)
			{
				pdst = dst+k;
				psrc = src+j;
				alpha = (psrc->a * thiz->gc.alpha) >> 8;
				pdst->r = FTK_ALPHA_1(psrc->r, pdst->r, alpha);
				pdst->g = FTK_ALPHA_1(psrc->g, pdst->g, alpha);
				pdst->b = FTK_ALPHA_1(psrc->b, pdst->b, alpha);
			}
			src += bitmap_width;
			dst += width;
		}
	}
	else
	{
		for(i = y; i < h; i++)
		{
			for(j = x, k = xoffset; j < w; j++, k++)
			{
				pdst = dst+k;
				psrc = src+j;
				if(src[j].a == 0xff)
				{
					dst[k] = src[j];
				}
				else
				{
					alpha = psrc->a; 
					pdst->r = FTK_ALPHA_1(psrc->r, pdst->r, alpha);
					pdst->g = FTK_ALPHA_1(psrc->g, pdst->g, alpha);
					pdst->b = FTK_ALPHA_1(psrc->b, pdst->b, alpha);
				}
			}
			src += bitmap_width;
			dst += width;
		}
	}

	return RET_OK;
}

Ret ftk_canvas_draw_bitmap_normal(FtkCanvas* thiz, FtkBitmap* bitmap, FtkRect* src_r, int xoffset, int yoffset)
{
	return ftk_canvas_draw_bitmap(thiz, bitmap, src_r->x, src_r->y, src_r->width, src_r->height, xoffset, yoffset);
}

FtkBitmap* ftk_canvas_bitmap(FtkCanvas* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->bitmap;
}

int ftk_canvas_font_height(FtkCanvas* thiz)
{
	return ftk_font_height(thiz->gc.font);
}

int ftk_canvas_get_extent(FtkCanvas* thiz, const char* str, int len)
{
	return_val_if_fail(thiz != NULL && str != NULL && thiz->gc.font != NULL, 0);
	
	return ftk_font_get_extent(thiz->gc.font, str, len);
}

int ftk_canvas_get_char_extent(FtkCanvas* thiz, unsigned short unicode)
{
	return ftk_font_get_char_extent(thiz->gc.font, unicode);
}

const char* ftk_canvas_calc_str_visible_range(FtkCanvas* thiz, const char* start, int vstart, int vend, int width)
{
	return ftk_font_calc_str_visible_range(thiz->gc.font, start, vstart, vend, width, NULL);
}

void ftk_canvas_destroy(FtkCanvas* thiz)
{
	if(thiz != NULL)
	{
		ftk_gc_reset(&thiz->gc);
		ftk_bitmap_unref(thiz->bitmap);
		FTK_ZFREE(thiz, sizeof(*thiz));
	}

	return;
}

static Ret ftk_canvas_fill_background_four_corner(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap)
{
	int i = 0;
	int k = 0;
	int ox = 0;
	int oy = 0;
	int ow = 0;
	int oh = 0;
	FtkGc gc = {0};
	FtkColor* bits = NULL;
	FtkColor* pixel = NULL;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int tile_w = FTK_MIN(bw, w) >> 1;
	int tile_h = FTK_MIN(bh, h) >> 1;
	
	gc.mask = FTK_GC_FG;
	ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, tile_w, tile_h, x, y);
	ftk_canvas_draw_bitmap(canvas, bitmap, bw - tile_w, 0, tile_w, tile_h, x + w - tile_w, y);
	ftk_canvas_draw_bitmap(canvas, bitmap, 0, bh - tile_h, tile_w, tile_h, x, y + h - tile_h);
	ftk_canvas_draw_bitmap(canvas, bitmap, bw - tile_w, bh - tile_h, tile_w, tile_h, x + w - tile_w, y + h - tile_h);

	if(bw < w)
	{
		ox = x + tile_w;
		ow = w - 2 * tile_w;
		bits = ftk_bitmap_bits(bitmap) + tile_w;
		for(i = 0; i < tile_h; i++)
		{
			pixel = canvas->bits + (y + i) * canvas->width + ox; 
			for(k = 0; k < ow; k++, pixel++)
			{
				PUT_PIXEL(pixel, bits, bits->a);
			}
			bits += bw;
		}
		
		oy = y + tile_h;
		oh = h - 2 * tile_h;
		for(i = 0; i < oh; i++)
		{
			pixel = canvas->bits + (oy + i) * canvas->width + ox;
			for(k = 0; k < ow; k++, pixel++)
			{
				PUT_PIXEL(pixel, bits, bits->a);
			}
		}
	
		oy = y + h - tile_h;
		bits = ftk_bitmap_bits(bitmap) + (bh - tile_h) * bw + tile_w;
		for(i = 0; i < tile_h; i++)
		{
			pixel = canvas->bits + (oy + i) * canvas->width + ox;
			for(k = 0; k < ow; k++, pixel++)
			{
				PUT_PIXEL(pixel, bits, bits->a);
			}
			bits += bw;
		}
	}

	if(bh < h)
	{
		oy = y + tile_h;
		oh = h - 2 * tile_h;
		bits = ftk_bitmap_bits(bitmap) + bw * tile_h;
		for(i = 0; i < tile_w; i++)
		{
			pixel = canvas->bits + oy * canvas->width + x + i;
			for(k = 0; k < oh; k++, pixel+=canvas->width)
			{
				PUT_PIXEL(pixel, bits, bits->a);
			}
			bits++;
		}
		
		ox = x + w - tile_w;
		bits = ftk_bitmap_bits(bitmap) + bw * tile_h + FTK_HALF(bw);
		for(i = 0; i < tile_w; i++)
		{
			pixel = canvas->bits + oy * canvas->width + ox + i;
			for(k = 0; k < oh; k++, pixel+=canvas->width)
			{
				PUT_PIXEL(pixel, bits, bits->a);
			}
			bits++;
		}
	}

	return RET_OK;
}

static Ret ftk_canvas_fill_background_normal(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	return ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, w, h, x, y);
}

static Ret ftk_canvas_fill_background_tile(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap)
{
	int dx = 0;
	int dy = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	if(bw > w && bh > h)
	{
		return ftk_canvas_fill_background_normal(canvas, x, y, w, h, bitmap);
	}

	for(dy = 0; dy < h; dy += bh)
	{
		for(dx = 0; dx < w; dx += bw)
		{
			int draw_w = (dx + bw) < w ? bw : w - dx;
			int draw_h = (dy + bh) < h ? bh : h - dy;

			ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, draw_w, draw_h, x + dx, y + dy);
		}
	}
	
	return RET_OK;
}

static Ret ftk_canvas_fill_background_center(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int ox = bw < w ? x + (w - bw)/2 : x;
	int oy = bh < h ? y + (h - bh)/2 : y;
	int bx = bw < w ? 0 : (bw - w)/2;
	int by = bh < h ? 0 : (bh - h)/2;
	
	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	return ftk_canvas_draw_bitmap(canvas, bitmap, bx, by, w, h, ox, oy);
}

Ret ftk_canvas_draw_bg_image(FtkCanvas* canvas, FtkBitmap* bitmap, FtkBgStyle style, int x, int y, int w, int h)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(canvas != NULL && bitmap != NULL, ret);

	switch(style)
	{
		case FTK_BG_TILE: 
			ret = ftk_canvas_fill_background_tile(canvas, x, y, w, h, bitmap);break;
		case FTK_BG_CENTER: 
			ret = ftk_canvas_fill_background_center(canvas, x, y, w, h, bitmap);break;
		case FTK_BG_FOUR_CORNER:
			ret = ftk_canvas_fill_background_four_corner(canvas, x, y, w, h, bitmap);break;
		default:
			ret = ftk_canvas_fill_background_normal(canvas, x, y, w, h, bitmap);break;
	}

	return ret;
}

static Ret ftk_canvas_draw_bitmap_resize(FtkCanvas* thiz, FtkBitmap* src_i, FtkRect* src_r, FtkRect* dst_r)
{
	FtkColor* s = NULL;
	FtkColor* d = NULL;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;
	FtkColor* psrc = NULL;
	FtkColor* pdst = NULL;
	FtkBitmap* dst_i = thiz->bitmap;	
	int i=0, j=0, m=0, n=0;
	int dst_i_w = ftk_bitmap_width(dst_i); 
	int dst_i_h = ftk_bitmap_height(dst_i);
	int src_i_w = ftk_bitmap_width(src_i);
	int src_i_h = ftk_bitmap_height(src_i);

	int dst_x = dst_r->x;
	int dst_y = dst_r->y;
	int dst_w = dst_r->width;
	int dst_h = dst_r->height;

	int src_x = src_r->x;
	int src_y = src_r->y;
	int src_w = src_r->width;
	int src_h = src_r->height;

	int scale_w = (src_w << 8)/dst_w;
	int scale_h = (src_h << 8)/dst_h;

	return_val_if_fail(dst_i != NULL && ftk_bitmap_bits(dst_i) != NULL && dst_r != NULL, RET_FAIL);
	return_val_if_fail(src_i != NULL && ftk_bitmap_bits(src_i) != NULL && src_r != NULL, RET_FAIL);
	
	return_val_if_fail(dst_x >= 0 && dst_w > 0 && (dst_x + dst_w) <= dst_i_w, RET_FAIL);
	return_val_if_fail(dst_y >= 0 && dst_h > 0 && (dst_y + dst_h) <= dst_i_h, RET_FAIL);
	
	return_val_if_fail(src_x >= 0 && src_w > 0 && (src_x + src_w) <= src_i_w, RET_FAIL);
	return_val_if_fail(src_y >= 0 && src_h > 0 && (src_y + src_h) <= src_i_h, RET_FAIL);
	
	src = ftk_bitmap_bits(src_i) + src_y * src_i_w + src_x;
	dst = ftk_bitmap_bits(dst_i) + dst_y * dst_i_w + dst_x;
        
	if(thiz->gc.mask & FTK_GC_ALPHA)
	{
		int alpha = thiz->gc.alpha; 
		for(j=0; j<dst_h; j++)
		{
			n = (j*scale_h) >> 8;
			pdst = dst + j * dst_i_w;
			psrc = src + n * src_i_w;

			for(i=0 ; i<dst_w ; i++)
			{
				m = (i*scale_w) >> 8;
				d = pdst+i;
				s = psrc+m;
				
				d->r = FTK_ALPHA_1(s->r, d->r, alpha);
				d->g = FTK_ALPHA_1(s->g, d->g, alpha);
				d->b = FTK_ALPHA_1(s->b, d->b, alpha);
			}	
		}
	}
	else
	{
		for(j=0; j<dst_h; j++)
		{
			n = (j*scale_h) >> 8;
			pdst = dst + j * dst_i_w;
			psrc = src + n * src_i_w;

			for(i=0 ; i<dst_w ; i++)
			{
				m = (i*scale_w) >> 8;

				pdst[i] = psrc[m];
			}	
		}
	}

	return 	RET_OK;
}

Ret ftk_canvas_draw_bitmap_ex(FtkCanvas* thiz, FtkBitmap* bitmap, FtkRect* src_r, FtkRect* dst_r, int alpha)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL && bitmap != NULL && dst_r != NULL && src_r != NULL, RET_FAIL);

	if(alpha == 0)
	{
		return RET_OK;
	}
	else if(alpha == 0xff)
	{
		thiz->gc.alpha = 0xff;
		thiz->gc.mask = thiz->gc.mask & (~FTK_GC_ALPHA);
	}
	else
	{
		FtkGc gc;
		gc.mask = FTK_GC_ALPHA;
		gc.alpha = alpha;
		ftk_canvas_set_gc(thiz, &gc);
	}

	if(dst_r->width == src_r->width && dst_r->height == src_r->height)
	{
		ret = ftk_canvas_draw_bitmap_normal(thiz, bitmap, src_r, dst_r->x, dst_r->y);
	}
	else
	{
		ret = ftk_canvas_draw_bitmap_resize(thiz, bitmap, src_r, dst_r);
	}

	return ret;
}
