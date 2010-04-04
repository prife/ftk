/*
 * File: ftk_util.c    
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

#include "ftk_typedef.h"

/*UTF8-related functions are copied from glib.*/

#define UTF8_COMPUTE(Char, Mask, Len)					      \
  if (Char < 128)							      \
    {									      \
      Len = 1;								      \
      Mask = 0x7f;							      \
    }									      \
  else if ((Char & 0xe0) == 0xc0)					      \
    {									      \
      Len = 2;								      \
      Mask = 0x1f;							      \
    }									      \
  else if ((Char & 0xf0) == 0xe0)					      \
    {									      \
      Len = 3;								      \
      Mask = 0x0f;							      \
    }									      \
  else if ((Char & 0xf8) == 0xf0)					      \
    {									      \
      Len = 4;								      \
      Mask = 0x07;							      \
    }									      \
  else if ((Char & 0xfc) == 0xf8)					      \
    {									      \
      Len = 5;								      \
      Mask = 0x03;							      \
    }									      \
  else if ((Char & 0xfe) == 0xfc)					      \
    {									      \
      Len = 6;								      \
      Mask = 0x01;							      \
    }									      \
  else									      \
    Len = -1;

#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))
   

#define UTF8_GET(Result, Chars, Count, Mask, Len)			      \
  (Result) = (Chars)[0] & (Mask);					      \
  for ((Count) = 1; (Count) < (Len); ++(Count))				      \
    {									      \
      if (((Chars)[(Count)] & 0xc0) != 0x80)				      \
	{								      \
	  (Result) = -1;						      \
	  break;							      \
	}								      \
      (Result) <<= 6;							      \
      (Result) |= ((Chars)[(Count)] & 0x3f);				      \
    }

#define UNICODE_VALID(Char)                   \
    ((Char) < 0x110000 &&                     \
     (((Char) & 0xFFFFF800) != 0xD800) &&     \
     ((Char) < 0xFDD0 || (Char) > 0xFDEF) &&  \
     ((Char) & 0xFFFE) != 0xFFFE)
static const char utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

const char * const g_utf8_skip = utf8_skip_data;
#define g_utf8_next_char(p) (char *)((p) + g_utf8_skip[*(const unsigned char *)(p)])

unsigned short utf8_get_char (const char *p, const char** next)
{
  int i, mask = 0, len;
  unsigned short result;
  unsigned char c = (unsigned char) *p;

  UTF8_COMPUTE (c, mask, len);
  if (len == -1)
    return (unsigned short)-1;
  UTF8_GET (result, p, i, mask, len);

  if(next != NULL)
  {
    *next = g_utf8_next_char(p);
  }

  return result;
}

unsigned short utf8_get_prev_char (const char *p, const char** prev)
{
	int i = 0;
	for(i = 1; i < 8; i++)
	{
		unsigned char val = p[-i];
		if((val & 0x80) && !(val & 0x40))
		{
			continue;
		}
		else
		{
			if(prev != NULL)
			{
				*prev = p-i;
			}
			return utf8_get_char(p-i, NULL);
		}
	}

	if(prev != NULL)
	{
		*prev = p;
	}

	return 0;
}

int utf8_count_char(const char *str, size_t length)
{
	int nr = 0;
	const char* iter = str;
	return_val_if_fail(str != NULL, 0);

	while(utf8_get_char(iter, &iter) && (iter - str) <= (int)length)
	{
		nr++;
	}

	return nr;
}

FtkColor ftk_parse_color( const char* value)
{
	int a = 0xff;
	int r = 0;
	int g = 0;
	int b = 0;
	FtkColor color = {0};
	
	sscanf(value, "%02x%02x%02x%02x", &a, &r, &g, &b);
	color.a = a;
	color.r = r;
	color.g = g;
	color.b = b;

	return color;
}

const char* ftk_normalize_path(char* path)
{
	char* p = path;
	return_val_if_fail(path != NULL, NULL);
	
	while(*p)
	{
		if(*p == '/' || *p == '\\')
		{
			*p = FTK_PATH_DELIM;
		}
		p++;
	}

	return path;
}

