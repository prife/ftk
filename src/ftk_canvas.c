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

Ret ftk_canvas_reset_gc(FtkCanvas* thiz, FtkGc* gc)
{
	return_val_if_fail(thiz != NULL && gc != NULL, RET_FAIL);

	ftk_gc_reset(&thiz->gc);
	
	ftk_gc_copy(&thiz->gc, gc);

	return ftk_canvas_sync_gc(thiz);
}

Ret ftk_canvas_set_gc(FtkCanvas* thiz, FtkGc* gc)
{
	return_val_if_fail(thiz != NULL && gc != NULL, RET_FAIL);

	ftk_gc_copy(&thiz->gc, gc);

	return ftk_canvas_sync_gc(thiz);
}

FtkGc* ftk_canvas_get_gc(FtkCanvas* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return &thiz->gc;
}

Ret    ftk_canvas_set_clip_rect(FtkCanvas* thiz, FtkRect* rect)
{
	if(rect != NULL)
	{
		FtkRegion region;
		region.rect = *rect;
		region.next = NULL;

		return ftk_canvas_set_clip(thiz, &region);
	}
	else
	{
		return ftk_canvas_set_clip(thiz, NULL);
	}
}

Ret    ftk_canvas_set_clip_region(FtkCanvas* thiz, FtkRegion* region)
{
	return ftk_canvas_set_clip(thiz, region);
}

Ret    ftk_cavans_get_clip_region(FtkCanvas* thiz, FtkRegion** region)
{
	/*TODO*/
	return RET_OK;
}

Ret ftk_canvas_draw_vline(FtkCanvas* thiz, size_t x, size_t y, size_t h)
{
	return ftk_canvas_draw_line(thiz, x, y, x, y + h);
}

Ret ftk_canvas_draw_hline(FtkCanvas* thiz, size_t x, size_t y, size_t w)
{
	return ftk_canvas_draw_line(thiz, x, y, x + w, y);
}

Ret ftk_canvas_draw_bitmap_simple(FtkCanvas* thiz, FtkBitmap* b, size_t x, size_t y, size_t w, size_t h, size_t ox, size_t oy)
{
	FtkRect src_r;
	FtkRect dst_r;

	src_r.x = x;
	src_r.y = y;
	dst_r.x = ox;
	dst_r.y = oy;
	src_r.width = dst_r.width = w;
	src_r.height = dst_r.height = h;

	return ftk_canvas_draw_bitmap(thiz, b, &src_r, &dst_r, 0xff);
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

const char* ftk_canvas_calc_str_visible_range(FtkCanvas* thiz, const char* start, 
	int vstart, int vend, size_t width)
{
	return ftk_font_calc_str_visible_range(thiz->gc.font, start, vstart, vend, width, NULL);
}

static Ret ftk_canvas_fill_background_four_corner(FtkCanvas* thiz, size_t x, size_t y, 
	size_t w, size_t h, FtkBitmap* bitmap)
{
	int i = 0;
	size_t ox = 0;
	size_t oy = 0;
	size_t ow = 0;
	size_t oh = 0;
	FtkGc gc = {0};
	FtkColor* bits = NULL;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int tile_w = FTK_MIN(bw, w) >> 1;
	int tile_h = FTK_MIN(bh, h) >> 1;
	FtkColor fg  = thiz->gc.fg;
	
	gc.mask = FTK_GC_FG;
	ftk_canvas_draw_bitmap_simple(thiz, bitmap, 0, 0, tile_w, tile_h, x, y);
	ftk_canvas_draw_bitmap_simple(thiz, bitmap, bw - tile_w, 0, tile_w, tile_h, x + w - tile_w, y);
	ftk_canvas_draw_bitmap_simple(thiz, bitmap, 0, bh - tile_h, tile_w, tile_h, x, y + h - tile_h);
	ftk_canvas_draw_bitmap_simple(thiz, bitmap, bw - tile_w, bh - tile_h, tile_w, tile_h, x + w - tile_w, y + h - tile_h);

	if(bw < w)
	{
		ox = x + tile_w;
		ow = w - 2 * tile_w;
		bits = ftk_bitmap_bits(bitmap) + tile_w;
		for(i = 0; i < tile_h; i++)
		{
			thiz->gc.fg = *bits;	
			ftk_canvas_sync_gc(thiz);
			ftk_canvas_draw_hline(thiz, ox, y + i, ow);
			bits += bw;
		}
		
		oy = y + tile_h;
		oh = h - 2 * tile_h;
		thiz->gc.fg = *bits;	
		ftk_canvas_sync_gc(thiz);
		ftk_canvas_draw_rect(thiz, ox, oy, ow, oh, 0, 1); 
	
		oy = y + h - tile_h;
		bits = ftk_bitmap_bits(bitmap) + (bh - tile_h) * bw + tile_w;
		for(i = 0; i < tile_h; i++)
		{
			thiz->gc.fg = *bits;
			ftk_canvas_sync_gc(thiz);
			ftk_canvas_draw_hline(thiz, ox, (oy + i), ow);
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
			thiz->gc.fg = *bits;	
			ftk_canvas_sync_gc(thiz);
			ftk_canvas_draw_vline(thiz, x + i, oy, oh);
			bits++;
		}
		
		ox = x + w - tile_w;
		bits = ftk_bitmap_bits(bitmap) + bw * tile_h + FTK_HALF(bw);
		for(i = 0; i < tile_w; i++)
		{
			thiz->gc.fg = *bits;	
			ftk_canvas_sync_gc(thiz);
			ftk_canvas_draw_vline(thiz, ox + i, oy, oh);
			bits++;
		}
	}
	thiz->gc.fg = fg;
	ftk_canvas_sync_gc(thiz);

	return RET_OK;
}

static Ret ftk_canvas_fill_background_normal(FtkCanvas* thiz, size_t x, size_t y, 
	size_t w, size_t h, FtkBitmap* bitmap)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	return ftk_canvas_draw_bitmap_simple(thiz, bitmap, 0, 0, w, h, x, y);
}

static Ret ftk_canvas_fill_background_tile(FtkCanvas* thiz, size_t x, size_t y, 
	size_t w, size_t h, FtkBitmap* bitmap)
{
	int dx = 0;
	int dy = 0;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	if(bw > w && bh > h)
	{
		return ftk_canvas_fill_background_normal(thiz, x, y, w, h, bitmap);
	}

	for(dy = 0; dy < h; dy += bh)
	{
		for(dx = 0; dx < w; dx += bw)
		{
			int draw_w = (dx + bw) < w ? bw : w - dx;
			int draw_h = (dy + bh) < h ? bh : h - dy;

			ftk_canvas_draw_bitmap_simple(thiz, bitmap, 0, 0, draw_w, draw_h, x + dx, y + dy);
		}
	}
	
	return RET_OK;
}

static Ret ftk_canvas_fill_background_center(FtkCanvas* thiz, size_t x, size_t y, size_t w, size_t h, FtkBitmap* bitmap)
{
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	size_t ox = bw < w ? x + (w - bw)/2 : x;
	size_t oy = bh < h ? y + (h - bh)/2 : y;
	int bx = bw < w ? 0 : (bw - w)/2;
	int by = bh < h ? 0 : (bh - h)/2;
	
	w = FTK_MIN(bw, w);
	h = FTK_MIN(bh, h);

	return ftk_canvas_draw_bitmap_simple(thiz, bitmap, bx, by, w, h, ox, oy);
}

Ret ftk_canvas_draw_bg_image(FtkCanvas* thiz, FtkBitmap* bitmap, FtkBgStyle style, 
	size_t x, size_t y, size_t w, size_t h)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL && bitmap != NULL, ret);

	switch(style)
	{
		case FTK_BG_TILE: 
			ret = ftk_canvas_fill_background_tile(thiz, x, y, w, h, bitmap);break;
		case FTK_BG_CENTER: 
			ret = ftk_canvas_fill_background_center(thiz, x, y, w, h, bitmap);break;
		case FTK_BG_FOUR_CORNER:
			ret = ftk_canvas_fill_background_four_corner(thiz, x, y, w, h, bitmap);break;
		default:
			ret = ftk_canvas_fill_background_normal(thiz, x, y, w, h, bitmap);break;
	}

	return ret;
}

Ret ftk_canvas_show(FtkCanvas* thiz, FtkDisplay* display, FtkRect* rect, int ox, int oy)
{
	FtkBitmap* bitmap = NULL;

	ftk_canvas_lock_buffer(thiz, &bitmap);
	ftk_display_update_and_notify(display, bitmap, rect, ox, oy);
	ftk_canvas_unlock_buffer(thiz);

	return RET_OK;
}
