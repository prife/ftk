/*
 * File: ftk_bitmap.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   Ftk bitmap
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
