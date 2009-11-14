/*
 * File: ftk_display.h    
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
#ifndef FTK_DISPLAY_H
#define FTK_DISPLAY_H

#include "ftk_bitmap.h"

struct _FtkDisplay;
typedef struct _FtkDisplay FtkDisplay;

typedef Ret (*FtkDisplayUpdate)(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset);
typedef int (*FtkDisplayWidth)(FtkDisplay* thiz);
typedef int (*FtkDisplayHeight)(FtkDisplay* thiz);
typedef int (*FtkDisplayBitsPerPixel)(FtkDisplay* thiz);
typedef Ret (*FtkDisplaySnap)(FtkDisplay* thiz, FtkBitmap** bitmap);
typedef void (*FtkDisplayDestroy)(FtkDisplay* thiz);

struct _FtkDisplay
{
	FtkDisplayUpdate       update;
	FtkDisplayWidth        width;
	FtkDisplayHeight       height;
	FtkDisplayBitsPerPixel bits_per_pixel;
	FtkDisplaySnap         snap;
	FtkDisplayDestroy destroy;

	char priv[1];
};

static inline Ret ftk_display_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	return_val_if_fail(thiz != NULL && thiz->update != NULL, RET_FAIL);

	return thiz->update(thiz, bitmap, rect, xoffset, yoffset);
}

static inline int ftk_display_width(FtkDisplay* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->width != NULL, RET_FAIL);

	return thiz->width(thiz);
}

static inline int ftk_display_height(FtkDisplay* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->height != NULL, RET_FAIL);

	return thiz->height(thiz);
}

static inline int ftk_display_bits_per_pixel(FtkDisplay* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->bits_per_pixel != NULL, RET_FAIL);

	return thiz->bits_per_pixel(thiz);
}

static inline Ret ftk_display_snap(FtkDisplay* thiz, FtkBitmap** bitmap)
{
	return_val_if_fail(thiz != NULL && thiz->snap != NULL, RET_FAIL);

	return thiz->snap(thiz, bitmap);
}

static inline void ftk_display_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
		thiz->destroy(thiz);
	}

	return;
}

#endif/*FTK_DISPLAY_H*/
