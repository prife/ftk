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

#define PUT_PIXEL(pdst, alpha) \
	do\
	{\
		if(alpha != 0xff)\
		{\
			FtkColor* color = &(thiz->gc.fg);\
			FTK_ALPHA(color, pdst, alpha);\
		}\
		else\
		{\
			pdst[0] = thiz->gc.fg;\
		}\
	}while(0);

Ret ftk_canvas_put_pixel(FtkCanvas* thiz, int x, int y, FtkColor val)
{
	int width = 0;
	int height = 0;
	FtkColor* bits = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
	bits   = ftk_bitmap_bits(thiz->bitmap);
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

	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
	bits   = ftk_bitmap_bits(thiz->bitmap);
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
	return_val_if_fail(thiz != NULL, RET_FAIL);
	
	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
	bits   = ftk_bitmap_bits(thiz->bitmap);
	return_val_if_fail(bits != NULL && y < height, RET_FAIL);
	alpha = thiz->gc.mask & FTK_GC_ALPHA ? thiz->gc.alpha :  thiz->gc.fg.a;

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	h = (y + h) < height ? h : (height - y);
	
	if(thiz->gc.mask & FTK_GC_LINE_MASK)
	{
		unsigned int line_mask = thiz->gc.line_mask;
		for(i = 0; i < h; i++)
		{
			if(FTK_MASK_BITS(line_mask, i))
			{
				pdst = bits + width * (y + i) + x;
				PUT_PIXEL(pdst, alpha);
			}
		}
	}
	else
	{
		for(i = 0; i < h; i++)
		{
			pdst = bits + width * (y + i) + x;
			PUT_PIXEL(pdst, alpha);
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
	
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
	bits   = ftk_bitmap_bits(thiz->bitmap);
	return_val_if_fail(bits != NULL && x < width, RET_FAIL);
	return_val_if_fail(y < height, RET_FAIL);	
	alpha = thiz->gc.mask & FTK_GC_ALPHA ? thiz->gc.alpha :  thiz->gc.fg.a;

	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	w = (x + w) < width ? w : width - x;
	bits += y * width;
	if(thiz->gc.mask & FTK_GC_LINE_MASK)
	{
		unsigned int line_mask = thiz->gc.line_mask;
		for(i = 0; i < w; i++)
		{
			if(FTK_MASK_BITS(line_mask, i))
			{
				pdst = bits + x + i;
				PUT_PIXEL(pdst, alpha);
			}
		}
	}
	else
	{
		for(i = 0; i < w; i++)
		{
			pdst = bits + x + i;
			PUT_PIXEL(pdst, alpha);
		}
	}

	return RET_OK;
}

Ret ftk_canvas_draw_rect(FtkCanvas* thiz, int x, int y, int w, int h, int fill)
{
	int i = 0;
	int width  = 0;
	int height = 0;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
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
	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
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
	width  = ftk_bitmap_width(thiz->bitmap);
	height = ftk_bitmap_height(thiz->bitmap);
	bits   = ftk_bitmap_bits(thiz->bitmap);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	color.a = 0xff;
	fg = thiz->gc.fg;

	/*FIXME: vcenter_offset maybe not correct.*/
	vcenter_offset = ftk_font_height(thiz->gc.font)/3;
	while(*iter && (iter - str) < len)
	{
		int offset = 0;
		code = utf8_get_char(iter, &iter);

		if(code == 0x20)
		{
			x += FTK_SPACE_WIDTH;
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
					color.r = ((fg.r * data) + (bg.r * (0xff - data)))/0xff;
					color.g = ((fg.g * data) + (bg.g * (0xff - data)))/0xff;
					color.b = ((fg.b * data) + (bg.b * (0xff - data)))/0xff;
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

Ret ftk_canvas_set_bitmap(FtkCanvas* thiz, FtkBitmap* bitmap, int x, int y, int w, int h, int xoffset, int yoffset)
{
	int i = 0;
	int j = 0;
	int k = 0;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;

	int width  = ftk_bitmap_width(thiz->bitmap);
	int height = ftk_bitmap_height(thiz->bitmap);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < bitmap_width, RET_FAIL);
	return_val_if_fail(y < bitmap_height, RET_FAIL);
	return_val_if_fail(xoffset < width, RET_FAIL);
	return_val_if_fail(yoffset < height, RET_FAIL);

	src = ftk_bitmap_bits(bitmap);
	dst = ftk_bitmap_bits(thiz->bitmap);

	w = (x + w) < bitmap_width  ? w : bitmap_width - x;
	w = (xoffset + w) < width  ? w : width  - xoffset;
	h = (y + h) < bitmap_height ? h : bitmap_height - y;
	h = (yoffset + h) < height ? h : height - yoffset;
	
	w += x;
	h += y;

	src += y * bitmap_width;
	dst += yoffset * width;

	for(i = y; i < h; i++)
	{
		for(j = x, k = xoffset; j < w; j++, k++)
		{
			dst[k] = src[j];
		}
		src += bitmap_width;
		dst += width;
	}

	return RET_OK;
}

/* add by woodysu*/
/* the bitmap is Src , the FtkCanvas.bitmap is Dst*/
Ret ftk_canvas_draw_bitmap_zoom(FtkCanvas* thiz, FtkBitmap* bitmap, int x, int y, int w, int h, unsigned char alpha)
{
	int i=0, j=0, m=0, n=0;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;
	int width  = ftk_bitmap_width(thiz->bitmap); //Canvas width
	int height = ftk_bitmap_height(thiz->bitmap);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < width, RET_FAIL);
	return_val_if_fail(y < height, RET_FAIL);

	src = ftk_bitmap_bits(bitmap);
	dst = ftk_bitmap_bits(thiz->bitmap);
        
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
				if(src[n*bitmap_width+m].a == 0xff)
				{
					dst[(j+y)*width+x+i]=src[n*bitmap_width+m];
				}	
				else
				{
					dst[(j+y)*width+x+i].r = (src[n*bitmap_width+m].r * src[n*bitmap_width+m].a + dst[(j+y)*width+x+i].r * (0xff - src[n*bitmap_width+m].a)) >> 8;
					dst[(j+y)*width+x+i].g = (src[n*bitmap_width+m].g * src[n*bitmap_width+m].a + dst[(j+y)*width+x+i].g * (0xff - src[n*bitmap_width+m].a)) >> 8;
					dst[(j+y)*width+x+i].b = (src[n*bitmap_width+m].b * src[n*bitmap_width+m].a + dst[(j+y)*width+x+i].b * (0xff - src[n*bitmap_width+m].a)) >> 8;
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
	unsigned char alpha = 0;
	FtkColor* src = NULL;
	FtkColor* dst = NULL;
	int width  = ftk_bitmap_width(thiz->bitmap);
	int height = ftk_bitmap_height(thiz->bitmap);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);

	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	return_val_if_fail(x < bitmap_width, RET_FAIL);
	return_val_if_fail(y < bitmap_height, RET_FAIL);
	return_val_if_fail(xoffset < width, RET_FAIL);
	return_val_if_fail(yoffset < height, RET_FAIL);

	src = ftk_bitmap_bits(bitmap);
	dst = ftk_bitmap_bits(thiz->bitmap);

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
				alpha = (src[j].a * thiz->gc.alpha) >> 8;
				dst[k].r = (src[j].r * alpha + dst[k].r * (0xff - alpha)) >> 8;
				dst[k].g = (src[j].g * alpha + dst[k].g * (0xff - alpha)) >> 8;
				dst[k].b = (src[j].b * alpha + dst[k].b * (0xff - alpha)) >> 8;
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
				if(src[j].a == 0xff)
				{
					dst[k] = src[j];
				}
				else
				{
					dst[k].r = (src[j].r * src[j].a + dst[k].r * (0xff - src[j].a)) >> 8;
					dst[k].g = (src[j].g * src[j].a + dst[k].g * (0xff - src[j].a)) >> 8;
					dst[k].b = (src[j].b * src[j].a + dst[k].b * (0xff - src[j].a)) >> 8;
				}
			}
			src += bitmap_width;
			dst += width;
		}
	}

	return RET_OK;
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
	int extent = 0;
	FtkGlyph glyph = {0};
	unsigned short code = 0;
	const char* iter = str;
	return_val_if_fail(thiz != NULL && str != NULL && thiz->gc.font != NULL, 0);
	
	len = len >= 0 ? len : (int)strlen(str);
	while(*iter && (iter - str) < len)
	{
		code = utf8_get_char(iter, &iter);
		if(code == ' ')
		{
			glyph.x = 0;
			glyph.w = FTK_SPACE_WIDTH - 1;
		}
		else if(code == '\r' || code == '\n' || ftk_font_lookup(thiz->gc.font, code, &glyph) != RET_OK) 
		{
			continue;
		}

	//	fprintf(stderr, "%s: %d %d\n", __func__, glyph.x, glyph.w);
		extent += glyph.x + glyph.w + 1;
	}
	//fprintf(stderr, "%s: %s %d\n", __func__, str, extent);
	return extent;
}

int ftk_font_get_char_extent(FtkFont* thiz, unsigned short unicode)
{
	int extent = 0;
	FtkGlyph glyph = {0};
	return_val_if_fail(thiz != NULL, 0);

	if(unicode == ' ')
	{
		return FTK_SPACE_WIDTH;
	}

	if(unicode == '\r' || unicode == '\n' || ftk_font_lookup(thiz, unicode, &glyph) != RET_OK)
	{

		return 0;
	}

	extent = glyph.x + glyph.w + 1;

	return extent;
}

int ftk_canvas_get_char_extent(FtkCanvas* thiz, unsigned short unicode)
{
	return ftk_font_get_char_extent(thiz->gc.font, unicode);
}

const char* ftk_font_calc_str_visible_range(FtkFont* thiz, const char* start, int vstart, int vend, int width)
{
	int extent = 0;
	unsigned short unicode = 0;
	const char* iter = NULL;
	const char* prev_iter = NULL;

	if(vstart >= 0)
	{
		iter = start + vstart;
		prev_iter = iter;
		while(width > 0)
		{
			prev_iter = iter;
			unicode = utf8_get_char(iter, &iter);
			if(unicode == 0 || unicode == 0xffff) break;

			extent = ftk_font_get_char_extent(thiz, unicode);
			if(extent > width) break;
			width -= extent;
			prev_iter = iter;
		}
	
		return prev_iter;
	}
	else if(vend > 0)
	{
		iter = start + vend;
		prev_iter = iter;
		while(width > 0 && iter >= start)
		{
			prev_iter = iter;
			if(iter <= start) break;
			unicode = utf8_get_prev_char(iter, &iter);
			if(unicode == 0 || iter < start) break;

			extent = ftk_font_get_char_extent(thiz, unicode);
			if(extent > width) break;
			width -= extent;
		}

		return prev_iter;
	}
		
	return start;
}

const char* ftk_canvas_calc_str_visible_range(FtkCanvas* thiz, const char* start, int vstart, int vend, int width)
{
	return ftk_font_calc_str_visible_range(thiz->gc.font, start, vstart, vend, width);
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
	int ox = 0;
	int oy = 0;
	int ow = 0;
	int oh = 0;
	FtkGc gc = {0};
	FtkColor* bits = NULL;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int tile_w = bw < w ? bw >> 1 : w >> 1;
	int tile_h = bh < h ? bh >> 1 : h >> 1;
	
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
			gc.fg = *bits;
			ftk_canvas_set_gc(canvas, &gc);
			ftk_canvas_draw_hline(canvas, ox, y + i, ow);
			bits += bw;
		}
		
		oy = y + tile_h;
		for(i = 0; i < (h - 2 * tile_h); i++)
		{
			ftk_canvas_draw_hline(canvas, ox, oy + i, ow);
		}
	
		oy = y + h - tile_h;
		bits = ftk_bitmap_bits(bitmap) + (bh - tile_h) * bw + tile_w;
		for(i = 0; i < tile_h; i++)
		{
			gc.fg = *bits;
			ftk_canvas_set_gc(canvas, &gc);
			ftk_canvas_draw_hline(canvas, ox, oy + i, ow);
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
			gc.fg = bits[i];
			ftk_canvas_set_gc(canvas, &gc);
			ftk_canvas_draw_vline(canvas, x + i, oy, oh);
		}
		
		ox = x + w - tile_w;
		bits = ftk_bitmap_bits(bitmap) + bw * tile_h;
		for(i = 0; i < tile_w; i++)
		{
			gc.fg = bits[ (bw >> 1) + i];
			ftk_canvas_set_gc(canvas, &gc);
			ftk_canvas_draw_vline(canvas, ox + i, oy, oh);
		}
	}

	return RET_OK;
}

static Ret ftk_canvas_fill_background_normal(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap, int set)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	if(set)
	{
		return ftk_canvas_set_bitmap(canvas, bitmap, 0, 0, w, h, x, y);
	}
	else
	{
		return ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, w, h, x, y);
	}
}

static Ret ftk_canvas_fill_background_tile(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap, int set)
{
	int dx = 0;
	int dy = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	if(bw > w && bh > h)
	{
		return ftk_canvas_fill_background_normal(canvas, x, y, w, h, bitmap, set);
	}

	for(dy = 0; dy < h; dy += bh)
	{
		for(dx = 0; dx < w; dx += bw)
		{
			int draw_w = (dx + bw) < w ? bw : w - dx;
			int draw_h = (dy + bh) < h ? bh : h - dy;

			if(set)
			{
				ftk_canvas_set_bitmap(canvas, bitmap, 0, 0, draw_w, draw_h, x + dx, y + dy);
			}
			else
			{
				ftk_canvas_draw_bitmap(canvas, bitmap, 0, 0, draw_w, draw_h, x + dx, y + dy);
			}
		}
	}
	
	return RET_OK;
}

static Ret ftk_canvas_fill_background_center(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap, int set)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int ox = bw < w ? x + (w - bw)/2 : x;
	int oy = bh < h ? y + (h - bh)/2 : y;
	int bx = bw < w ? 0 : (bw - w)/2;
	int by = bh < h ? 0 : (bh - h)/2;
	
	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	if(set)
	{
		return ftk_canvas_set_bitmap(canvas, bitmap, bx, by, w, h, ox, oy);
	}
	else
	{
		return ftk_canvas_draw_bitmap(canvas, bitmap, bx, by, w, h, ox, oy);
	}
}

Ret ftk_canvas_set_bg_image(FtkCanvas* canvas, FtkBitmap* bitmap, FtkBgStyle style, int x, int y, int w, int h)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(canvas != NULL && bitmap != NULL, ret);

	switch(style)
	{
		case FTK_BG_TILE: 
			ret = ftk_canvas_fill_background_tile(canvas, x, y, w, h, bitmap, 1);break;
		case FTK_BG_CENTER: 
			ret = ftk_canvas_fill_background_center(canvas, x, y, w, h, bitmap, 1);break;
		case FTK_BG_FOUR_CORNER:
			ret = ftk_canvas_fill_background_four_corner(canvas, x, y, w, h, bitmap);break;
		default:
			ret = ftk_canvas_fill_background_normal(canvas, x, y, w, h, bitmap, 1);break;
	}

	return ret;
}

Ret ftk_canvas_draw_bg_image(FtkCanvas* canvas, FtkBitmap* bitmap, FtkBgStyle style, int x, int y, int w, int h)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(canvas != NULL && bitmap != NULL, ret);

	switch(style)
	{
		case FTK_BG_TILE: 
			ret = ftk_canvas_fill_background_tile(canvas, x, y, w, h, bitmap, 0);break;
		case FTK_BG_CENTER: 
			ret = ftk_canvas_fill_background_center(canvas, x, y, w, h, bitmap, 0);break;
		case FTK_BG_FOUR_CORNER:
			ret = ftk_canvas_fill_background_four_corner(canvas, x, y, w, h, bitmap);break;
		default:
			ret = ftk_canvas_fill_background_normal(canvas, x, y, w, h, bitmap, 0);break;
	}

	return ret;
}
