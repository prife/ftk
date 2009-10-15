/*
 * File: ftk_gc.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:  graphic context 
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
#ifndef FTK_GC_H
#define FTK_GC_H
#include "ftk_font.h"
#include "ftk_bitmap.h"

#define FTK_GC_BG     1
#define FTK_GC_FG     2
#define FTK_GC_FONT   4
#define FTK_GC_BITMAP 8

typedef struct _FtkGc
{
	int ref;
	unsigned int mask;
	FtkColor   bg;
	FtkColor   fg;
	FtkFont*   font;
	FtkBitmap* bitmap;
}FtkGc;

static inline Ret ftk_gc_copy(FtkGc* dst, FtkGc* src)
{
	return_val_if_fail(dst != NULL && src != NULL, RET_FAIL);

	dst->mask |= src->mask;
	if(src->mask & FTK_GC_BG)
	{
		dst->bg = src->bg;
	}

	if(src->mask & FTK_GC_FG)
	{
		dst->fg = src->fg;
	}

	if(src->mask & FTK_GC_FONT)
	{
		dst->font = src->font;
	}

	if(src->mask & FTK_GC_BITMAP)
	{
		if(dst->bitmap != NULL)
		{
			ftk_bitmap_unref(dst->bitmap);
		}
		dst->bitmap = src->bitmap;
		if(dst->bitmap != NULL)
		{
			ftk_bitmap_ref(dst->bitmap);
		}
	}

	return RET_OK;
}

static inline Ret ftk_gc_reset(FtkGc* gc)
{
	if(gc != NULL)
	{
		if(gc->mask & FTK_GC_BITMAP)
		{
			ftk_bitmap_unref(gc->bitmap);
		}
		memset(gc, 0x00, sizeof(*gc));
	}

	return RET_OK;
}

#endif/*FTK_GC_H*/

