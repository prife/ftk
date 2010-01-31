/*
 * File: ftk_input_method_manager.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   input method manager.
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
 * 2010-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_allocator.h"
#include "ftk_input_method_manager.h"

#define FTK_INPUT_METHOD_MAX_NR 6

struct _FtkInputMethodManager
{
	size_t nr;
	FtkInputMethod* methods[FTK_INPUT_METHOD_MAX_NR+1];
};

FtkInputMethodManager* ftk_input_method_manager_create(void)
{
	FtkInputMethodManager* thiz = FTK_ZALLOC(sizeof(FtkInputMethodManager));

	return thiz;
}

size_t  ftk_input_method_manager_count(FtkInputMethodManager* thiz)
{
	return thiz != NULL ? thiz->nr : 0;
}

Ret  ftk_input_method_manager_get(FtkInputMethodManager* thiz, size_t index, FtkInputMethod** im)
{
	return_val_if_fail(thiz != NULL && index < thiz->nr && im != NULL, RET_FAIL);

	*im = thiz->methods[index];

	return RET_OK;
}

Ret  ftk_input_method_manager_register(FtkInputMethodManager* thiz, FtkInputMethod* im)
{
	return_val_if_fail(thiz != NULL && im != NULL && thiz->nr < FTK_INPUT_METHOD_MAX_NR, RET_FAIL);

	thiz->methods[thiz->nr] = im;
	thiz->nr++;

	return RET_OK;
}

Ret  ftk_input_method_manager_unregister(FtkInputMethodManager* thiz, FtkInputMethod* im)
{
	size_t i = 0;
	return_val_if_fail(thiz != NULL && im != NULL, RET_FAIL);

	for(i = 0; i < thiz->nr; i++)
	{
		if(thiz->methods[i] == im)
		{
			ftk_input_method_destroy(thiz->methods[i]);
			thiz->methods[i] = NULL;
			break;
		}
	}
	
	for(; i < thiz->nr; i++)
	{
		thiz->methods[i] = thiz->methods[i+1];
	}

	return RET_OK;
}

void ftk_input_method_manager_destroy(FtkInputMethodManager* thiz)
{
	size_t i = 0;
	if(thiz != NULL)
	{
		for(i = 0; i < thiz->nr; i++)
		{
			ftk_input_method_destroy(thiz->methods[i]);
			thiz->methods[i] = NULL;
		}
	}

	return;
}

