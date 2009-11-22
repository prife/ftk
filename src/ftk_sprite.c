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
#include "ftk_globals.h"

struct _FtkSprite
{
	int x;
	int y;
	int visible;
	FtkBitmap* icon;
	FtkBitmap* snap;
	void* listener_ctx;
	FtkListener listener;
};

FtkSprite* ftk_sprite_create(void)
{
	FtkSprite* thiz = FTK_ZALLOC(sizeof(FtkSprite));
	
	return thiz;
}

static Ret  ftk_sprite_backup(FtkSprite* thiz)
{
	FtkDisplay* display = ftk_default_display();
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->snap != NULL, RET_FAIL);
	
	return ftk_display_snap(display, thiz->x, thiz->y, thiz->snap);
}

static Ret  ftk_sprite_restore(FtkSprite* thiz)
{
	FtkRect rect = {0};
	FtkDisplay* display = ftk_default_display();
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->snap != NULL, RET_FAIL);
	
	rect.width = ftk_bitmap_width(thiz->snap);
	rect.height = ftk_bitmap_height(thiz->snap);

	return ftk_display_update(display, thiz->snap, &rect, thiz->x, thiz->y);
}

static Ret  ftk_sprite_paint(FtkSprite* thiz)
{
	FtkRect rect = {0};
	FtkDisplay* display = ftk_default_display();
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->icon != NULL, RET_FAIL);
	
	rect.width = ftk_bitmap_width(thiz->icon);
	rect.height = ftk_bitmap_height(thiz->icon);

	return ftk_display_update(display, thiz->icon, &rect, thiz->x, thiz->y);
}

Ret  ftk_sprite_set_icon(FtkSprite* thiz, FtkBitmap* icon)
{
	Ret ret = RET_FAIL;
	FtkColor bg = {.a = 0xff};
	return_val_if_fail(thiz != NULL && icon != NULL, RET_FAIL);
	
	thiz->icon = icon;
	if(thiz->snap != NULL)
	{
		ftk_bitmap_unref(thiz->snap);
	}
	thiz->snap = ftk_bitmap_create(ftk_bitmap_width(icon), ftk_bitmap_height(icon), bg);

	if(thiz->visible)
	{
		ftk_sprite_restore(thiz);
		ret = ftk_sprite_paint(thiz);
	}

	return ret;
}

Ret  ftk_sprite_show(FtkSprite* thiz, int show)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->visible != show, RET_FAIL);

	thiz->visible = show;
	
	if(thiz->visible)
	{
		ftk_sprite_backup(thiz);
		ret = ftk_sprite_paint(thiz);
	}
	else
	{
		ret = ftk_sprite_restore(thiz);
	}

	return ret;
}

int  ftk_sprite_is_visible(FtkSprite* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->visible;
}

Ret  ftk_sprite_move(FtkSprite* thiz, int x, int y)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL, RET_FAIL);
	
	if(thiz->x == x && thiz->y == y)
	{
		return RET_OK;
	}

	if(thiz->visible)
	{
		ftk_sprite_restore(thiz);	
	}

	thiz->x = x;
	thiz->y = y;
	
	if(thiz->visible)
	{
		ftk_sprite_backup(thiz);
		ret = ftk_sprite_paint(thiz);
	}

	FTK_CALL_LISTENER(thiz->listener, thiz->listener_ctx, thiz);

	return ret;
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

	thiz->listener = listener;
	thiz->listener_ctx = ctx;

	return RET_OK;
}

void ftk_sprite_destroy(FtkSprite* thiz)
{
	if(thiz != NULL)
	{
		ftk_bitmap_unref(thiz->icon);
		ftk_bitmap_unref(thiz->snap);
		FTK_ZFREE(thiz, sizeof(*thiz));
	}

	return;
}


