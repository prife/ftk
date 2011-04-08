/*
 * File: ftk_font.c
 *
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: common used font functions.
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
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
 * 2010-07-18 Li XianJing <xianjimli@hotmail.com> created
 * 2011-04-08 Li XianJing <xianjimli@hotmail.com> add ftk_font_cache.
 *
 */

#include "ftk_font.h"
#include "ftk_util.h"

int ftk_font_get_char_extent(FtkFont* thiz, unsigned short unicode)
{
	int extent = 0;
	FtkGlyph glyph = {0};
	return_val_if_fail(thiz != NULL, 0);

	if(unicode == ' ')
	{
		return FTK_SPACE_WIDTH;
	}
	else if(unicode == '\t')
	{
		return FTK_SPACE_WIDTH * FTK_TAB_WIDTH;
	}
	else if(unicode == '\r' || unicode == '\n')
	{
		return 0;
	}

	if(ftk_font_lookup(thiz, unicode, &glyph) == RET_OK)
	{
		extent = glyph.x + glyph.w + 1;
	}

	return extent;
}

int ftk_font_get_extent(FtkFont* thiz, const char* str, int len)
{
	int extent = 0;
	unsigned short code = 0;
	const char* iter = str;
	return_val_if_fail(thiz != NULL && str != NULL, 0);
	
	len = len >= 0 ? len : (int)strlen(str);
	while(*iter && (iter - str) < len)
	{
		code = utf8_get_char(iter, &iter);
		if(code == 0 || code == 0xffff)
		{
			break;
		}
		extent += ftk_font_get_char_extent(thiz, code);
	}

	return extent;
}

const char* ftk_font_calc_str_visible_range(FtkFont* thiz, 
	const char* start, int vstart, int vend, int width, int* ret_extent)
{
	int extent = 0;
	int line_extent = 0;
	unsigned short unicode = 0;
	const char* iter = NULL;
	const char* prev_iter = NULL;

	if(vstart >= 0)
	{
		iter = start + vstart;
		prev_iter = iter;
		while(width > 0)
		{
			prev_iter = iter;
			unicode = utf8_get_char(iter, &iter);
			if(unicode == '\r') continue;
			if(unicode == 0 || unicode == 0xffff)
			{
				break;
			}
			else if( unicode == '\n') 
			{	
				prev_iter = iter;
				break;
			}

			extent = ftk_font_get_char_extent(thiz, unicode);
			if(extent > width) break;
			width -= extent;
			line_extent += extent;
			prev_iter = iter;
		}
		if(ret_extent != NULL) 
			*ret_extent = line_extent;
	
		return prev_iter;
	}
	else if(vend > 0)
	{
		iter = start + vend;
		prev_iter = iter;
		while(width > 0 && iter >= start)
		{
			prev_iter = iter;
			if(iter <= start) break;
			unicode = utf8_get_prev_char(iter, &iter);
			if(unicode == '\r') continue;
			if(unicode == 0 || unicode == 0xffff || unicode == '\n')
			{
			//	prev_iter = iter;
				break;
			}
			extent = ftk_font_get_char_extent(thiz, unicode);
			if(extent > width) break;
			width -= extent;
			line_extent += extent;
		}

		if(ret_extent != NULL) 
			*ret_extent = line_extent;

		return prev_iter;
	}
		
	return start;
}

//////////////////////////////////////////////////////

typedef struct _PrivInfo
{
	FtkFont* font;
	FtkGlyph* glyphs;
	FtkGlyph* free_glyphs;
	FtkGlyph** glyphs_ptr;
	size_t glyph_nr;
	size_t font_height;
	size_t max_glyph_nr;
	size_t one_glyph_size;
}PrivInfo;

static FtkGlyph* ftk_font_cache_alloc(FtkFont* thiz)
{
	char* end = NULL;
	FtkGlyph* p = NULL;
	DECL_PRIV(thiz, priv);
	
	end = (char*)priv->glyphs + priv->max_glyph_nr * priv->one_glyph_size;

	if((char*)priv->free_glyphs < end)
	{
		p = priv->free_glyphs;
		priv->free_glyphs = (FtkGlyph*) ((char*) priv->free_glyphs + priv->one_glyph_size);
	}

	return p;
}

static Ret ftk_font_cache_add(FtkFont* thiz, FtkGlyph* glyph)
{
	int i = 0;
	int pos = 0;
	FtkGlyph* p = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL, -1);

	if(glyph->w > priv->font_height || glyph->h > priv->font_height)
	{
		ftk_logd("%s: %p is too large to cache\n", __func__, glyph->code);
		return RET_FAIL;
	}

	for(pos = 0; pos < priv->glyph_nr; pos++)
	{
		if(priv->glyphs_ptr[pos]->code >= glyph->code)
		{
			break;
		}
	}

	if(priv->glyph_nr < priv->max_glyph_nr)
	{
		i = priv->glyph_nr;
		for(; i > pos; i--)
		{
			priv->glyphs_ptr[i] = priv->glyphs_ptr[i-1];
		}
		
		priv->glyph_nr++;
		priv->glyphs_ptr[pos] = ftk_font_cache_alloc(thiz);
	}
	else
	{
		if(pos == priv->glyph_nr)
		{
			pos = priv->glyph_nr - 1;
		}
	}

	p = priv->glyphs_ptr[pos];
	*p = *glyph;
	p->data = (unsigned char*)p + sizeof(FtkGlyph);

	memcpy(p->data, glyph->data, glyph->w * glyph->h);
	ftk_logd("%s add %p at %d\n", __func__, p->code, pos);

	return RET_OK;
}

static Ret ftk_font_cache_lookup (FtkFont* thiz, unsigned short code, FtkGlyph* glyph)
{
	int low    = 0;
	int mid    = 0;
	int high   = 0;
	int result = 0;
	FtkGlyph* p = NULL;
	Ret ret =  RET_FAIL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL, -1);

	high = priv->glyph_nr-1;
	while(low <= high)
	{
		mid  = low + ((high - low) >> 1);
		p = priv->glyphs_ptr[mid];
		result = p->code - code;
		if(result == 0)
		{
			*glyph = *p;
			ftk_logd("%s find %p at %d\n", __func__, p->code, mid);
			return RET_OK;
		}
		else if(result < 0)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	if((ret = ftk_font_lookup(priv->font, code, glyph)) == RET_OK)
	{
		if(ftk_font_cache_add(thiz, glyph) == RET_OK)
		{
//for test
#if 0
			FtkGlyph g = {0};
			assert(ftk_font_cache_lookup(thiz, code, &g) == RET_OK);
			assert(g.code == code);
			assert(memcmp(&g, glyph, sizeof(g) - 4) == 0);
			assert(memcmp(g.data, glyph->data, g.w * g.h) == 0);
#endif			
		}
	}

	return RET_OK;
}

static int      ftk_font_cache_height(FtkFont* thiz)
{
		DECL_PRIV(thiz, priv);

		return ftk_font_height(priv->font);
}


static void		ftk_font_cache_destroy(FtkFont* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		FTK_FREE(priv->glyphs);
		FTK_FREE(priv->glyphs_ptr);
		ftk_font_unref(priv->font);
	}

	return;
}

FtkFont* ftk_font_cache_create (FtkFont* font, size_t max_glyph_nr)
{
	FtkFont* thiz = NULL;
	size_t font_height = ftk_font_height(font);
	return_val_if_fail(font != NULL && font_height > 0, NULL);

	thiz = FTK_ZALLOC(sizeof(FtkFont) + sizeof(PrivInfo));
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		thiz->ref = 1;
		thiz->height = ftk_font_cache_height;
		thiz->lookup = ftk_font_cache_lookup;
		thiz->destroy = ftk_font_cache_destroy;

		priv->font = font;
		ftk_font_ref(font);
		
		priv->glyph_nr = 0;
		priv->font_height = font_height;
		priv->max_glyph_nr = max_glyph_nr;
		priv->one_glyph_size = sizeof(FtkGlyph) + font_height * font_height ;

		priv->glyphs = FTK_ZALLOC(max_glyph_nr * priv->one_glyph_size);
		priv->glyphs_ptr = FTK_ZALLOC(max_glyph_nr * sizeof(FtkGlyph*));
		priv->free_glyphs = priv->glyphs;
		
		ftk_logd("%s: max_glyph_nr=%d memsize=%d\n", __func__, 
			max_glyph_nr, max_glyph_nr * priv->one_glyph_size);
	}

	return thiz;
}

