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

	if(unicode < 0x80 && thiz->char_extent_cache[unicode] > 0)
	{
		extent = thiz->char_extent_cache[unicode];
	}
	else if(ftk_font_lookup(thiz, unicode, &glyph) == RET_OK)
	{
		extent = glyph.x + glyph.w + 1;
		if(unicode < 0x80)
		{
			thiz->char_extent_cache[unicode] = extent;
		}
	}

	return extent;
}

const char* ftk_font_calc_str_visible_range(FtkFont* thiz, const char* start, int vstart, int vend, int width, int* ret_extent)
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

