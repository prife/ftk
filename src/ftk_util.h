/*
 * File: ftk_util.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   common used functions.
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_UTIL_H
#define FTK_UTIL_H

#include "ftk_typedef.h"
#include "ftk_allocator.h"

FTK_BEGIN_DECLS

unsigned short utf8_get_char (const char *p, const char** next);
unsigned short utf8_get_prev_char (const char *p, const char** prev);
int utf8_count_char(const char *begin, size_t length);
FtkColor ftk_parse_color( const char* value);

FTK_END_DECLS

#endif/*FTK_UTIL_H*/

