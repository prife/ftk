/*
 * File: ftk_font.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   font 
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

#include "ftk_log.h"
#include "fontdata.h"
#include "ftk_font.h"
#include "ftk_util.h"

struct _FtkFont
{
	size_t size;
	char* filename;
	FontData* fontdata;
};

static Ret ftk_font_load(FtkFont* thiz, const char* filename)
{
	int length = 0;
	char* buffer = read_file(filename, &length);

	if(buffer != NULL)
	{
		thiz->fontdata = font_data_load(buffer, length);
		ftk_logd("%s: author=%s\n", __func__, font_data_get_author(thiz->fontdata));
		ftk_logd("%s: family=%s\n", __func__, font_data_get_family(thiz->fontdata));
		ftk_logd("%s: style =%s\n", __func__, font_data_get_style(thiz->fontdata));
		ftk_logd("%s: w=%d h=%d\n", __func__, font_data_get_width(thiz->fontdata), font_data_get_height(thiz->fontdata));
	}
	
	return RET_OK;
}

FtkFont*  ftk_font_create (const char* filename, size_t size)
{
	Ret ret = RET_FAIL;
	FtkFont* thiz = (FtkFont*)FTK_ZALLOC(sizeof(FtkFont));
	if(thiz != NULL)
	{
		thiz->size = size;
		if((ret = ftk_font_load(thiz, filename)) != RET_OK)
		{
			FTK_ZFREE(thiz, sizeof(FtkFont));
		}
		else
		{
			thiz->filename = strdup(filename);
		}
	}

	if(ret != RET_OK)
	{
		ftk_font_destroy(thiz);
		thiz = NULL;
	}

	return thiz;
}

int      ftk_font_height(FtkFont* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return font_data_get_height(thiz->fontdata);
}

Ret ftk_font_lookup (FtkFont* thiz, unsigned short code, FtkGlyph* glyph)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	return font_data_get_glyph(thiz->fontdata, code, (Glyph*)glyph);
}

void      ftk_font_destroy(FtkFont* thiz)
{
	if(thiz != NULL)
	{
		FTK_FREE(thiz->filename);
		font_data_destroy(thiz->fontdata);
		FTK_ZFREE(thiz, sizeof(thiz));
	}

	return;
}

