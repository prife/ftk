/*
 * File: ftk_theme.c   
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   theme
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
 * 2009-12-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_theme.h"

typedef struct _FtkWidgetTheme
{
	FtkColor   bg[FTK_WIDGET_STATE_NR];
	FtkColor   fg[FTK_WIDGET_STATE_NR];
	FtkBitmap* bg_bitmap[FTK_WIDGET_STATE_NR];
}FtkWidgetTheme;

struct _FtkTheme
{
	FtkWidgetTheme widget_themes[FTK_WIDGET_TYPE_NR];	
};

static Ret ftk_theme_init_default(void)
{
	return RET_OK;
}

FtkTheme*  ftk_theme_create(void)
{
	FtkTheme* thiz = FTK_ZALLOC(sizeof(FtkTheme));

	if(thiz != NULL)
	{
		ftk_theme_init_default(thiz);
	}

	return thiz;
}

Ret        ftk_theme_parse_data(FtkTheme* thiz, const char* data, size_t length)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_FAIL);

	return RET_OK;
}

Ret        ftk_theme_parse_file(FtkTheme* thiz, const char* filename)
{
	FtkMmap* m = NULL;
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL && filename != NULL, RET_FAIL);
	
	m = ftk_mmap_create(filename, 0, -1);
	return_val_if_fail(m != NULL, RET_FAIL);
	ret = ftk_theme_parse_data(thiz, ftk_mmap_data(m), ftk_mmap_length(m));
	ftk_mmap_destroy(m);

	return ret;
}

FtkBitmap* ftk_theme_get_bg(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	return thiz->widget_themes[type].bg_bitmap[state];
}

FtkColor   ftk_theme_get_bg_color(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	return thiz->widget_themes[type].bg[state];
}

FtkColor   ftk_theme_get_fg_color(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state);
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	return thiz->widget_themes[type].fg[state];
}

void       ftk_theme_destroy(FtkTheme* thiz)
{
	if(thiz != NULL)
	{
		size_t i = 0;
		size_t j = 0;

		for(i = 0; i < FTK_WIDGET_TYPE_NR; i++)
		{
			for(j = 0; j < FTK_WIDGET_STATE_NR; j++)
			{
				if(thiz->widget_themes[i].bg_bitmap[j] != NULL)
				{
					ftk_bitmap_unref(thiz->widget_themes[i].bg_bitmap[j]);
					thiz->widget_themes[i].bg_bitmap[j] = NULL;
				}
			}
		}
	}

	return;
}


