/*
 * File: ftk_font.h    
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
#ifndef FTK_FONT_H
#define FTK_FONT_H

#include "ftk_typedef.h"

typedef struct _FtkGlyph
{
	char x;
	char y;
	unsigned char w;
	unsigned char h;
	unsigned short code;
	unsigned char* data;
}FtkGlyph;

struct _FtkFont;
typedef struct _FtkFont FtkFont;

FtkFont* ftk_font_create (const char* filename, size_t size);
int      ftk_font_height(FtkFont* thiz);
Ret      ftk_font_lookup (FtkFont* thiz, unsigned short code, FtkGlyph* glyph);
void     ftk_font_destroy(FtkFont* thiz);

#endif/*FTK_FONT_H*/

