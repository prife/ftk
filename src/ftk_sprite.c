/*
 * File:    ftk_sprite.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   screen sprite
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
 * 2009-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_sprite.h"

struct _FtkSprite
{
	int x;
	int y;
	int visible;
	FtkBitmap* icon;
	void* listener_ctx;
	FtkListener listener;
};

FtkSprite* ftk_sprite_create(void)
{
	FtkSprite* thiz = FTK_ZALLOC(sizeof(FtkSprite));

	return thiz;
}

static Ret  ftk_sprite_paint(FtkSprite* thiz, int force)
{
	return RET_OK;
}

Ret  ftk_sprite_set_icon(FtkSprite* thiz, FtkBitmap* icon)
{
	return_val_if_fail(thiz != NULL && icon != NULL, RET_FAIL);
	
	thiz->icon = icon;
	ftk_sprite_paint(thiz, 0);

	return RET_OK;
}

Ret  ftk_sprite_show(FtkSprite* thiz, int show)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->visible != show, RET_FAIL);

	thiz->visible = show;

	return ftk_sprite_paint(thiz, 1);
}

Ret  ftk_sprite_move(FtkSprite* thiz, int x, int y)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz->x = x;
	thiz->y = y;

	return ftk_sprite_paint(thiz, 0);
}

int  ftk_sprite_get_x(FtkSprite* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->x;
}

int  ftk_sprite_get_y(FtkSprite* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->y;
}

Ret  ftk_sprite_set_move_listener(FtkSprite* thiz, FtkListener listener, void* ctx)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz-listener = listener;
	thiz->listener_ctx = ctx;

	return RET_OK;
}

void ftk_sprite_destroy(FtkSprite* thiz)
{
	if(thiz != NULL)
	{
		ftk_bitmap_unref(thiz->icon);

		FTK_ZFREE(thiz, sizeof(*thiz));
	}

	return;
}


