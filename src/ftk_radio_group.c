/*
 * File: ftk_radio_group.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   radio group. 
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
 * 2009-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_radio_group.h"
#include "ftk_check_button.h"

static Ret ftk_radio_group_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return RET_OK;
}

static Ret ftk_radio_group_on_paint(FtkWidget* thiz)
{
	return RET_OK;
}

static void ftk_radio_group_destroy(FtkWidget* thiz)
{
	return;
}

FtkWidget* ftk_radio_group_create(FtkWidget* parent, int x, int y, int width, int height)
{
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
		
	if(thiz != NULL)
	{
		thiz->on_event = ftk_radio_group_on_event;
		thiz->on_paint = ftk_radio_group_on_paint;
		thiz->destroy  = ftk_radio_group_destroy;

		ftk_widget_init(thiz, FTK_RADIO_GROUP, 0, x, y, width, height, FTK_ATTR_TRANSPARENT);
		ftk_widget_append_child(parent, thiz);
	}

	return thiz;
}

Ret  ftk_radio_group_set_checked(FtkWidget* thiz, FtkWidget* radio)
{
	FtkWidget* iter = NULL;
	return_val_if_fail(thiz != NULL && radio != NULL, RET_FAIL);

	iter = thiz->children;

	while(iter != NULL)
	{
		ftk_check_button_set_checked(iter, iter == radio);

		iter = iter->next;
	}

	return RET_OK;
}

