/*
 * File: ftk_canvas.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   drawing kit.
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

#ifndef FTK_CANVAS_H
#define FTK_CANVAS_H

#include "ftk_gc.h"

typedef enum _FtkBgStyle
{
	FTK_BG_NORMAL = 0,
	FTK_BG_TILE,
	FTK_BG_CENTER,
	FTK_BG_FOUR_CORNER
}FtkBgStyle;

struct _FtkCanvas;
typedef struct _FtkCanvas FtkCanvas;

FtkCanvas* ftk_canvas_create(int w, int h, FtkColor clear_color);
Ret ftk_canvas_reset_gc(FtkCanvas* thiz, FtkGc* gc);
Ret ftk_canvas_set_gc(FtkCanvas* thiz, FtkGc* gc);
Ret ftk_canvas_draw_point(FtkCanvas* thiz, int x, int y);
Ret ftk_canvas_draw_line(FtkCanvas* thiz, int x1, int y1, int x2, int y2);
Ret ftk_canvas_draw_vline(FtkCanvas* thiz, int x, int y, int h);
Ret ftk_canvas_draw_hline(FtkCanvas* thiz, int x, int y, int w);
Ret ftk_canvas_draw_rect(FtkCanvas* thiz, int x, int y, int w, int h, int fill);
Ret ftk_canvas_draw_ellipse(FtkCanvas* thiz, int x, int y, int rx, int ry,int fill);
Ret ftk_canvas_draw_string(FtkCanvas* thiz, int x, int y, const char* str, int len);

Ret ftk_canvas_put_pixel(FtkCanvas* thiz, int x, int y, FtkColor val);
FtkColor* ftk_canvas_get_pixel(FtkCanvas* thiz, int x, int y);

Ret ftk_canvas_draw_bitmap(FtkCanvas* thiz, FtkBitmap* bitmap, int x, int y, int w, int h, int xoffset, int yoffset);

int ftk_canvas_font_height(FtkCanvas* thiz);
int ftk_canvas_get_extent(FtkCanvas* thiz, const char* str, int len);
int ftk_canvas_get_char_extent(FtkCanvas* thiz, unsigned short unicode);
const char* ftk_canvas_available(FtkCanvas* thiz, const char* str, int width, int* nr);
const char* ftk_canvas_compute_string_visible_range(FtkCanvas* thiz, 
	const char* start, int vstart, int vend, int width);
Ret ftk_canvas_fill_background_image(FtkCanvas* canvas, FtkBgStyle style, 
	int x, int y, int w, int h, FtkBitmap* bitmap);

FtkBitmap* ftk_canvas_bitmap(FtkCanvas* thiz);
void ftk_canvas_destroy(FtkCanvas* thiz);

#endif/*FTK_CANVAS_H*/

