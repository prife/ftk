/*
 * File: ftk_status_item.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   item on the status panel. 
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
 * 2009-10-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_button.h"
#include "ftk_status_item.h"


FtkWidget* ftk_status_item_create(int id, int width, int height)
{
	FtkWidget* thiz = ftk_button_create(id, 0, 0, width, height);

	return thiz;
}

Ret         ftk_status_item_set_position(FtkWidget* thiz, int pos)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz->priv_subclass[1] = (void*)pos;

	return RET_OK;
}

int         ftk_status_item_get_position(FtkWidget* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return (int)thiz->priv_subclass[1];
}

