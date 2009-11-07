#include "ftk_style.h"

static const FtkColor s_colors[FTK_COLOR_NR] =
{
	[FTK_COLOR_WINDOW]       = {.a=0xff, .r=0xff, .g=0xff,.b=0xff},
	[FTK_COLOR_BORDER]       = {.a=0xff, .r=0xf2, .g=0xee,.b=0xee},
	[FTK_COLOR_WINDOWFRAME]  = {.a=0xff, .r=0x00, .g=0x00,.b=0x00},
	[FTK_COLOR_WINDOWTEXT]   = {.a=0xff, .r=0x00, .g=0x00,.b=0x00},
	[FTK_COLOR_GRAYTEXT]     = {.a=0xff, .r=0xac, .g=0xa8,.b=0x99},
	[FTK_COLOR_HIGHLIGHT]    = {.a=0xff, .r=0xff, .g=0xbb,.b=0x00},
	[FTK_COLOR_BTNFACE]      = {.a=0xff, .r=0xf2, .g=0xee,.b=0xee},
	[FTK_COLOR_BTNHIGHLIGHT] = {.a=0xff, .r=0xff, .g=0xbb,.b=0x00},
	[FTK_COLOR_BTNSHADOW]    = {.a=0xff, .r=0xac, .g=0xa8,.b=0x99},
	[FTK_COLOR_BTNTEXT]      = {.a=0xff, .r=0x00, .g=0x00,.b=0x00},
	[FTK_COLOR_MENU_BG]      = {.a=0xff, .r=0xff, .g=0xff,.b=0xff},
	[FTK_COLOR_MENU_FG]      = {.a=0xff, .r=0xcc, .g=0xc9,.b=0xb8},
};

FtkColor ftk_style_get_color(FtkColorIndex index)
{
	return s_colors[index];
}

Ret ftk_style_fill_background(FtkCanvas* canvas, int x, int y, int w, int h, FtkBitmap* bitmap)
{
	int i = 0;
	int n = 0;
	int ox = 0;
	int oy = 0;
	int ow = 0;
	int oh = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkColor* bits = NULL;
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	int tile_w = bw < w ? bw >> 1 : w >> 1;
	int tile_h = bh < h ? bh >> 1 : h >> 1;
	
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
			gc.fg = bits[ bw/2 + i];
			ftk_canvas_set_gc(canvas, &gc);
			ftk_canvas_draw_vline(canvas, ox + i, oy, oh);
		}
	}

	return RET_OK;
}

Ret ftk_style_fill_button_background(FtkWidget* button)
{
	return RET_OK;
}

Ret ftk_style_fill_menu_item_button_background(FtkWidget* button)
{

	return RET_OK;
}
