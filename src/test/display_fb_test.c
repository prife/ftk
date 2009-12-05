/*
 * File: display_fb_test.c    
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

#include "ftk_display_fb.h"

int main(int argc, char* argv[])
{
	const char* filename = argv[1] != NULL ? argv[1] : FTK_FB_NAME;
	FtkDisplay* thiz = ftk_display_fb_create(filename);

	if(thiz != NULL)
	{
		FtkBitmap* bitmap = NULL;
		FtkColor color = {0xff, 0x00, 0xff, 0};
		FtkRect rect = {10, 10};
		rect.width = ftk_display_width(thiz);
		rect.height = ftk_display_height(thiz);

		bitmap = ftk_bitmap_create(rect.width, rect.height, color);

		ftk_display_destroy(thiz);
		ftk_bitmap_unref(bitmap);
	}

	return 0;
}
