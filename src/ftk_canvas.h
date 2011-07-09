/*
 * File: ftk_canvas.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   drawing kit.
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
 * 2011-04-16 Li XianJing <xianjimli@hotmail.com> change to interface.
 *
 */

#ifndef FTK_CANVAS_H
#define FTK_CANVAS_H

#include "ftk_gc.h"
#include "ftk_display.h"

FTK_BEGIN_DECLS

typedef enum _FtkBgStyle
{
	FTK_BG_NORMAL = 0,
	FTK_BG_TILE,
	FTK_BG_CENTER,
	FTK_BG_FOUR_CORNER
}FtkBgStyle;

struct _FtkCanvas;
typedef struct _FtkCanvas FtkCanvas;

typedef Ret (*FtkCanvasSyncGc)(FtkCanvas* thiz);
typedef Ret (*FtkCanvasSetClip)(FtkCanvas* thiz, FtkRegion* clip);
typedef Ret (*FtkCanvasDrawPixels)(FtkCanvas* thiz, FtkPoint* points, size_t nr);
typedef Ret (*FtkCanvasDrawLine)(FtkCanvas* thiz, size_t x1, size_t y1, size_t x2, size_t y2);
typedef Ret (*FtkCanvasClearRect)(FtkCanvas* thiz, size_t x, size_t y, size_t w, size_t h);
typedef Ret (*FtkCanvasDrawRect)(FtkCanvas* thiz, size_t x, size_t y, size_t w, size_t h, int round, int fill);
typedef Ret (*FtkCanvasDrawBitmap)(FtkCanvas* thiz, FtkBitmap* bitmap, FtkRect* s, FtkRect* d, int alpha);
typedef Ret (*FtkCanvasDrawString)(FtkCanvas* thiz, size_t x, size_t y, const char* str, int len, int vcenter);
typedef Ret (*FtkCanvasLockBuffer)(FtkCanvas* thiz, FtkBitmap** bitmap);
typedef Ret (*FtkCanvasUnlockBuffer)(FtkCanvas* thiz);
typedef void (*FtkCanvasDestroy)(FtkCanvas* thiz);

struct _FtkCanvas
{
	FtkGc gc;
	size_t width;
	size_t height;

	FtkCanvasSyncGc     sync_gc;
	FtkCanvasSetClip    set_clip;
	FtkCanvasDrawPixels draw_pixels;
	FtkCanvasDrawLine   draw_line;
	FtkCanvasClearRect  clear_rect;
	FtkCanvasDrawRect   draw_rect;
	FtkCanvasDrawBitmap draw_bitmap;
	FtkCanvasDrawString draw_string;
	FtkCanvasLockBuffer lock_buffer;
	FtkCanvasUnlockBuffer unlock_buffer;
	FtkCanvasDestroy      destroy;
	
	char priv[ZERO_LEN_ARRAY];
};

static inline Ret ftk_canvas_sync_gc(FtkCanvas* thiz)
{
	Ret ret = RET_FAIL;
	if(thiz != NULL && thiz->sync_gc != NULL)
	{
		ret = thiz->sync_gc(thiz);
	}

	return ret;
}

static inline Ret ftk_canvas_set_clip(FtkCanvas* thiz, FtkRegion* clip)
{
	Ret ret = RET_FAIL;
	if(thiz != NULL && thiz->set_clip != NULL)
	{
		ret = thiz->set_clip(thiz, clip);
	}

	return ret;
}

static inline Ret ftk_canvas_draw_pixels(FtkCanvas* thiz, FtkPoint* points, size_t nr)
{
	return_val_if_fail(thiz != NULL && thiz->draw_pixels != NULL && points != NULL, RET_FAIL);

	return thiz->draw_pixels(thiz, points, nr);
}

static inline Ret ftk_canvas_draw_line(FtkCanvas* thiz, size_t x1, size_t y1, size_t x2, size_t y2)
{
	return_val_if_fail(thiz != NULL && thiz->draw_line != NULL, RET_FAIL);

	return thiz->draw_line(thiz, x1, y1, x2, y2);
}

static inline Ret ftk_canvas_clear_rect(FtkCanvas* thiz, size_t x, size_t y, 
size_t w, size_t h)
{
	return_val_if_fail(thiz != NULL && thiz->clear_rect != NULL, RET_FAIL);

	return thiz->clear_rect(thiz, x, y, w, h);
}

static inline Ret ftk_canvas_draw_rect(FtkCanvas* thiz, size_t x, size_t y, 
	size_t w, size_t h, int round, int fill)
{
	return_val_if_fail(thiz != NULL && thiz->draw_rect != NULL, RET_FAIL);

	return thiz->draw_rect(thiz, x, y, w, h, round, fill);
}

static inline Ret ftk_canvas_draw_bitmap(FtkCanvas* thiz, FtkBitmap* bmp, 
	FtkRect* s, FtkRect* d, int alpha)
{
	return_val_if_fail(thiz != NULL && thiz->draw_bitmap != NULL, RET_FAIL);

	return thiz->draw_bitmap(thiz, bmp, s, d, alpha);
}

static inline Ret ftk_canvas_draw_string(FtkCanvas* thiz, size_t x, size_t y, 
	const char* str, int len, int vcenter)
{
	len = (len < 0 && str != NULL) ? (int)strlen(str) : len;
	return_val_if_fail(thiz != NULL && thiz->draw_string != NULL, RET_FAIL);

	return thiz->draw_string(thiz, x, y, str, len, vcenter);
}

static inline Ret ftk_canvas_lock_buffer(FtkCanvas* thiz, FtkBitmap** bitmap)
{
	return_val_if_fail(thiz != NULL && thiz->lock_buffer != NULL, RET_FAIL);

	return thiz->lock_buffer(thiz, bitmap);
}

static inline Ret ftk_canvas_unlock_buffer(FtkCanvas* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->unlock_buffer != NULL, RET_FAIL);

	return thiz->unlock_buffer(thiz);
}

static inline void ftk_canvas_destroy(FtkCanvas* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
		thiz->destroy(thiz);
	}

	return;
}

Ret    ftk_canvas_reset_gc(FtkCanvas* thiz, FtkGc* gc);
Ret    ftk_canvas_set_gc(FtkCanvas* thiz, FtkGc* gc);
FtkGc* ftk_canvas_get_gc(FtkCanvas* thiz);

Ret    ftk_canvas_set_clip_rect(FtkCanvas* thiz, FtkRect* rect);
Ret    ftk_canvas_set_clip_region(FtkCanvas* thiz, FtkRegion* region);
Ret    ftk_cavans_get_clip_region(FtkCanvas* thiz, FtkRegion** region);

Ret ftk_canvas_draw_vline(FtkCanvas* thiz, size_t x, size_t y, size_t h);
Ret ftk_canvas_draw_hline(FtkCanvas* thiz, size_t x, size_t y, size_t w);

int ftk_canvas_font_height(FtkCanvas* thiz);
int ftk_canvas_get_extent(FtkCanvas* thiz, const char* str, int len);
const char* ftk_canvas_calc_str_visible_range(FtkCanvas* thiz, const char* start, 
	int vstart, int vend, size_t width);

Ret ftk_canvas_draw_bitmap_simple(FtkCanvas* thiz, FtkBitmap* b, 
	size_t x, size_t y, size_t w, size_t h, size_t ox, size_t oy);
Ret ftk_canvas_draw_bg_image(FtkCanvas* thiz, FtkBitmap* bitmap, 
	FtkBgStyle style, size_t x, size_t y, size_t w, size_t h);

Ret ftk_canvas_show(FtkCanvas* thiz, FtkDisplay* display, FtkRect* rect, int ox, int oy);

FtkCanvas* ftk_canvas_create(size_t w, size_t h, FtkColor* clear_color);

FTK_END_DECLS

#endif/*FTK_CANVAS_H*/

