/*
 * File: canvas_test.c    
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
#include "ftk.h"

void test_misc(FtkDisplay* display, FtkFont* font)
{
	if(display != NULL)
	{
		int i = 0;
		int nr = 0;
		FtkGc gc = {0};
		int extent = 0;
		FtkColor color = {0x0, 0, 0, 0x0};
		FtkRect rect = {.x = 0, .y=0, .width=0, .height=0};
		rect.width = ftk_display_width(display);
		rect.height = ftk_display_height(display);
		gc.mask = FTK_GC_FG | FTK_GC_FONT;
		gc.fg.a = 0xff;
		gc.fg.r = 0x00;
		gc.fg.g = 0x00;
		gc.fg.b = 0xff;
		gc.font = font;

		FtkCanvas* thiz = ftk_canvas_create(rect.width, rect.height, color);
		ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
		for(i = 0; i < ftk_display_height(display); i++)
		{
			if(gc.fg.r < 0xff)
			{
				gc.fg.r++;
			}
			else
			{
				gc.fg.g++;
			}
			ftk_canvas_set_gc(thiz, &gc);
			ftk_canvas_draw_hline(thiz, 0, i, 320);
		}
		FtkBitmap* bitmap = ftk_bitmap_create(100, 100, color);
		ftk_canvas_draw_bitmap(thiz, bitmap, 0, 0, 100, 100, 100, 100);
		ftk_canvas_draw_string(thiz, 0, 240, " Jim is a Programmer.");
		gc.fg.b = 0xff;
		ftk_canvas_set_gc(thiz, &gc);
		ftk_canvas_draw_string(thiz, 0, 220, "李先静是一个程序员");
	
		ftk_canvas_draw_line(thiz, 0, 0, 100, 100);
		ftk_canvas_draw_line(thiz, 100, 100, 200, 0);
		ftk_canvas_draw_line(thiz, 100, 100, 0, 200);
		ftk_canvas_draw_ellipse(thiz, 50, 50, 50, 50, 1);
		ftk_canvas_draw_ellipse(thiz, 100, 50, 50, 50, 0);
		ftk_canvas_draw_ellipse(thiz, 150, 50, 50, 50, 0);
		ftk_canvas_draw_ellipse(thiz, 200, 50, 50, 50, 1);
		unsigned int line_mask = 0xaaaaaaaa;
		gc.line_mask = line_mask;
		gc.mask = FTK_GC_LINE_MASK;
		ftk_canvas_set_gc(thiz, &gc);
		ftk_canvas_draw_line(thiz, 100, 200, 200, 200);
		ftk_canvas_draw_line(thiz, 100, 200, 100, 300);
		ftk_canvas_draw_line(thiz, 100, 300, 200, 300);
		ftk_canvas_draw_line(thiz, 200, 200, 200, 300);
		ftk_canvas_draw_line(thiz, 200, 200, 300, 300);
		ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);

		assert(ftk_canvas_font_height(thiz) == 16);
		extent = ftk_canvas_get_extent(thiz, "李先静");
		assert(strcmp(ftk_canvas_available(thiz, "李先静是", extent, &nr), "是") == 0 && nr == 3);
		printf("extent=%d\n", ftk_canvas_get_extent(thiz, "李先静"));


		ftk_bitmap_unref(bitmap);
		ftk_canvas_destroy(thiz);
	}

	sleep(3);

	return;
}

void test_draw_point(FtkDisplay* display)
{
	int i = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkRect rect = {0};
	FtkColor color = {.a = 0xff};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	rect.width = width;
	rect.height = height;

	color.r = 0xff;
	color.a = 0xff;
	gc.fg = color;
	for(i = 0; i < width; i++)
	{
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_point(thiz, i, 10);
	}
	
	color.g = 0xff;
	color.r = 0;
	for(i = 0; i < width; i++)
	{
		color.a = 0xff - (0xff & i);
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_point(thiz, i, 20);
	}
	
	color.r = 0;
	color.g = 0;
	color.b = 0xff;
	color.a = 0xff;
	gc.fg = color;
	gc.mask |= FTK_GC_ALPHA;
	for(i = 0; i < width; i++)
	{
		gc.alpha = 0xff - (0xff & i);
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_point(thiz, i, 30);
	}

	ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
	
	ftk_canvas_destroy(thiz);

	sleep(3);

	return;
}

void test_draw_vline(FtkDisplay* display)
{
	int i = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkRect rect = {0};
	FtkColor color = {.a = 0xff};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	rect.width = width;
	rect.height = height;

	color.r = 0xff;
	color.a = 0xff;
	gc.fg = color;
	for(i = 0; i < width; i++)
	{
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_vline(thiz, i, 0, 20);
	}
	
	color.g = 0xff;
	color.r = 0;
	for(i = 0; i < width; i++)
	{
		color.a = 0xff - (0xff & i);
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_vline(thiz, i, 30, 20);
	}
	
	color.r = 0;
	color.g = 0;
	color.b = 0xff;
	color.a = 0xff;
	gc.fg = color;
	gc.mask |= FTK_GC_ALPHA;
	for(i = 0; i < width; i++)
	{
		gc.alpha = 0xff - (0xff & i);
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_vline(thiz, i, 60, 20);
	}

	ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
	
	ftk_canvas_destroy(thiz);
	sleep(3);

	return;
}

void test_draw_hline(FtkDisplay* display)
{
	int i = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkRect rect = {0};
	FtkColor color = {.a = 0xff};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	rect.width = width;
	rect.height = height;

	color.r = 0xff;
	color.a = 0xff;
	gc.fg = color;
	for(i = 0; i < height; i++)
	{
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_hline(thiz, 0, i, 20);
	}
	
	color.g = 0xff;
	color.r = 0;
	for(i = 0; i < height; i++)
	{
		color.a = 0xff - (0xff & i);
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_hline(thiz, 30, i, 20);
	}
	
	color.r = 0;
	color.g = 0;
	color.b = 0xff;
	color.a = 0xff;
	gc.fg = color;
	gc.mask |= FTK_GC_ALPHA;
	for(i = 0; i < height; i++)
	{
		gc.alpha = 0xff - (0xff & i);
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_hline(thiz, 60, i, 20);
	}

	ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
	
	ftk_canvas_destroy(thiz);

	sleep(3);

	return;
}

void test_draw_line(FtkDisplay* display)
{
	int i = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkRect rect = {0};
	FtkColor color = {.a = 0xff};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	rect.width = width;
	rect.height = height;

	color.r = 0xff;
	color.a = 0xff;
	gc.fg = color;
	for(i = 0; i < height/2; i++)
	{
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_line(thiz, 0, i, 20, i+10);
	}
	
	color.g = 0xff;
	color.r = 0;
	for(i = 0; i < height/2; i++)
	{
		color.a = 0xff - (0xff & i);
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_line(thiz, 30, i, 50, i+10);
	}
	
	color.r = 0;
	color.g = 0;
	color.b = 0xff;
	color.a = 0xff;
	gc.fg = color;
	gc.mask |= FTK_GC_ALPHA;
	for(i = 0; i < height/2; i++)
	{
		gc.alpha = 0xff - (0xff & i);
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_line(thiz, 60, i, 80, i+10);
	}

	ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
	
	ftk_canvas_destroy(thiz);

	sleep(3);

	return;
}

void test_draw_rect(FtkDisplay* display)
{
	int i = 0;
	FtkGc gc = {.mask = FTK_GC_FG};
	FtkRect rect = {0};
	FtkColor color = {.a = 0xff};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	rect.width = width;
	rect.height = height;

	color.g = 0xff;
	color.r = 0;
	for(i = 0; i < 0xff; i += 4)
	{
		color.a = 0xff;
		color.g = 0;
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_rect(thiz, 0, 0, width, height, 1);
		
		color.a = 0xff - i;
		color.g = 0xff;
		gc.fg = color;
		ftk_canvas_reset_gc(thiz, &gc);
		ftk_canvas_draw_rect(thiz, 0, 0, width, height, 1);
		ftk_display_update(display, ftk_canvas_bitmap(thiz), &rect, 0, 0);
		usleep(200000);
	}
	
	ftk_canvas_destroy(thiz);

	sleep(3);

	return;
}

void test_put_get_pixel(FtkDisplay* display)
{
	int i = 0;
	int j = 0;
	FtkColor color = {.a=0xff, .r=0xef, .g=0xdf, .b=0xcf};
	int width = ftk_display_width(display);
	int height = ftk_display_height(display);
	FtkCanvas* thiz = ftk_canvas_create(width, height, color);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			FtkColor* colorp = NULL;
			assert(ftk_canvas_put_pixel(thiz, j, i, color) == RET_OK);
			assert((colorp = ftk_canvas_get_pixel(thiz, j, i)) != NULL);
			assert(colorp->r == color.r);
			assert(colorp->g == color.g);
			assert(colorp->b == color.b);
			assert(colorp->a == color.a);
		}
	}

	ftk_canvas_destroy(thiz);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);

	FtkFont* font = ftk_default_font();
	FtkDisplay* display = ftk_default_display();

	test_put_get_pixel(display);
#if 1
	test_draw_point(display);
	test_draw_line(display);
	test_draw_hline(display);
	test_draw_vline(display);
#endif
	test_draw_rect(display);
	ftk_run();

	return 0;
}

