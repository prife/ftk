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
#include "ftk_globals.h"
#include "ftk_icon_cache.h"
#include "ftk_bitmap_factory.h"

typedef struct _FtkBitmapNamePair
{
	char* filename;
	FtkBitmap* bitmap;
}FtkBitmapNamePair;

#define FTK_ICON_CACHE_MAX 32

struct _FtkIconCache
{
	int nr;
	FtkBitmapNamePair pairs[FTK_ICON_CACHE_MAX];
};

static FtkBitmap* ftk_icon_cache_find(FtkIconCache* thiz, const char* filename)
{
	int i = 0;
	return_val_if_fail(thiz != NULL && filename != NULL, NULL);

	for(i = 0; i < thiz->nr; i++)
	{
		if(strcmp(filename, thiz->pairs[i].filename) == 0)
		{
			return thiz->pairs[i].bitmap;
		}
	}

	return NULL;
}

static FtkBitmap* ftk_icon_cache_real_load(FtkIconCache* thiz, const char* filename)
{
	char path[260] = {0};
	FtkBitmap* bitmap = NULL;
	return_val_if_fail(thiz != NULL && filename != NULL, NULL);

	snprintf(path, sizeof(path), "%s/icons/%s", DATA_DIR, filename);
	if((bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), path)) == NULL)
	{
		snprintf(path, sizeof(path), "./icons/%s", filename);
		bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), path);
	}
	
	return bitmap;
}

FtkIconCache* ftk_icon_cache_create(void)
{
	FtkIconCache* thiz = FTK_ZALLOC(sizeof(FtkIconCache));

	return thiz;
}

static Ret ftk_icon_cache_add(FtkIconCache* thiz, const char* filename, FtkBitmap* bitmap)
{
	if(thiz->nr < FTK_ICON_CACHE_MAX)
	{
		ftk_bitmap_ref(bitmap);
		thiz->pairs[thiz->nr].bitmap = bitmap;
		thiz->pairs[thiz->nr].filename = strdup(filename);
		thiz->nr++;
	}

	return RET_OK;
}

FtkBitmap* ftk_icon_cache_load(FtkIconCache* thiz, const char* filename)
{
	FtkBitmap* bitmap = NULL;
	return_val_if_fail(thiz != NULL && filename != NULL, NULL);

	if((bitmap = ftk_icon_cache_find(thiz, filename)) == NULL)
	{
		if((bitmap = ftk_icon_cache_real_load(thiz, filename)) != NULL)
		{
			ftk_icon_cache_add(thiz, filename, bitmap);
		}
	}

	return bitmap;
}

void ftk_icon_cache_destroy(FtkIconCache* thiz)
{
	int i = 0;
	
	if(thiz != NULL)
	{
		for(i = 0; i < thiz->nr; i++)
		{
			ftk_bitmap_unref(thiz->pairs[i].bitmap);
			FTK_FREE(thiz->pairs[i].filename);
		}

		FTK_ZFREE(thiz, sizeof(FtkIconCache));
	}

	return;
}


