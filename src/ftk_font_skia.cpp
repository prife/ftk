/*
 * File: ftk_font_skia.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   skia font.  
 *
 * Copyright (c) 2009 - 2011  Li XianJing <xianjimli@hotmail.com>
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
 * 2011-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_util.h"
#include "ftk_font.h"
#include "SkCanvas.h"

using namespace android;

typedef struct _PrivInfo
{
	SkPaint* paint;
	FtkGlyph glyph;
}PrivInfo;

static int      ftk_font_skia_height(FtkFont* thiz)
{
	DECL_PRIV(thiz, priv);

	return ftk_font_desc_get_size(thiz->font_desc);
}

static int ftk_font_skia_get_char_extent(FtkFont* thiz, unsigned short code)
{
	int width = 0;
	char utf8[8] = {0};
	DECL_PRIV(thiz, priv);
	SkScalar fwidth = 0;
	utf16_to_utf8(&code, 1, utf8, sizeof(utf8));

	fwidth = priv->paint->measureText(utf8, strlen(utf8));
	width = SkFixedFloor(SkScalarToFixed(fwidth));

//	ftk_logd("%s: %4x %s width=%d fwidth=%p\n", __func__, code, utf8, width, fwidth);

	return width;
}

static int ftk_font_skia_get_extent(FtkFont* thiz, const char* str, int len)
{
	int width = 0;
	char utf8[8] = {0};
	DECL_PRIV(thiz, priv);
	SkScalar fwidth = 0;
	
	fwidth = priv->paint->measureText(str, len);
	width = SkFixedFloor(SkScalarToFixed(fwidth));
	ftk_logd("%s: %s width=%d fwidth=%p\n", __func__, str, width, fwidth);

	return width;
}

static Ret ftk_font_skia_lookup (FtkFont* thiz, unsigned short code, FtkGlyph* glyph)
{
	int width = 0;
	char utf8[8] = {0};
	DECL_PRIV(thiz, priv);
	SkScalar fwidth = 0;
	utf16_to_utf8(&code, 1, utf8, sizeof(utf8));

	fwidth = priv->paint->measureText(utf8, strlen(utf8));
	width = SkFixedFloor(SkScalarToFixed(fwidth));

	priv->glyph.w = width;
	*glyph = priv->glyph;

	ftk_logd("%s: %4x %s width=%d fwidth=%p\n", __func__, code, utf8, width, fwidth);

	return RET_OK;
}

void      ftk_font_skia_destroy(FtkFont* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		delete priv->paint;
		ftk_font_desc_unref(thiz->font_desc);
		FTK_ZFREE(thiz, sizeof(*thiz) + sizeof(PrivInfo));
	}

	return;
}

extern "C" FtkFont* ftk_font_create (const char* filename, FtkFontDesc* font_desc)
{
	FtkFont* thiz = NULL;
	int size = ftk_font_desc_get_size(font_desc);
	return_val_if_fail(size > 0, NULL);

	thiz = (FtkFont*)FTK_ZALLOC(sizeof(FtkFont) + sizeof(PrivInfo));
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		
		thiz->ref = 1;
		thiz->height = ftk_font_skia_height;
		thiz->lookup = ftk_font_skia_lookup;
		thiz->destroy= ftk_font_skia_destroy;
		thiz->get_extent = ftk_font_skia_get_extent;
		thiz->get_char_extent = ftk_font_skia_get_char_extent;

		priv->paint = new SkPaint();
		priv->paint->setTextSize(SkIntToScalar(size));
		ftk_logd("%s: font_size=%d\n", __func__, size);
	
		ftk_font_desc_ref(font_desc);
		thiz->font_desc = font_desc;
		priv->glyph.data = (unsigned char*)FTK_ZALLOC(size * size);
		priv->glyph.w = size;
		priv->glyph.h = size;
	}

	return thiz;
}

