/*
 * File: ftk_icon_cache.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   cache to load icons.
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
 * 2009-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "ftk_icon_cache.h"
#include "ftk_bitmap_factory.h"

typedef struct _FtkBitmapNamePair
{
	FtkBitmap* bitmap;
	char* filename;
}FtkBitmapNamePair;

#define FTK_ICON_CACHE_MAX 32

struct _FtkIconCache
{
	int nr;
	FtkBitmapNamePair pairs[FTK_ICON_CACHE_MAX];
};

static FtkBitmap* ftk_icon_cache_find(FtkIconCache* thiz, const char* filename)
{
	return NULL;
}

static FtkBitmap* ftk_icon_cache_real_load(FtkIconCache* thiz, const char* filename)
{
	return NULL;
}

FtkIconCache* ftk_icon_cache_create(void)
{
	return NULL;
}

FtkBitmap* ftk_icon_cache_load(FtkIconCache* thiz, const char* filename)
{
	return NULL;
}

void ftk_icon_cache_destroy(FtkIconCache* thiz)
{
	return;
}


