/*
 * File: ftk_source_primary.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   gui primary source.
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
#include "ftk_source_primary.h"

#define MAX_EVENTS 128

typedef struct _PrivInfo
{
	int read_fd;
	int write_fd;
	FtkOnEvent on_event;
	void* user_data;
}PrivInfo;

static int ftk_source_primary_get_fd(FtkSource* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, -1);

	return priv->read_fd;
}

static int ftk_source_primary_check(FtkSource* thiz)
{
	return -1;
}

static Ret ftk_source_primary_dispatch(FtkSource* thiz)
{
	FtkEvent event = {0};
	DECL_PRIV(thiz, priv);
	int ret = read(priv->read_fd, &event, sizeof(FtkEvent));
	return_val_if_fail(ret == sizeof(FtkEvent), RET_REMOVE);

	if(priv->on_event != NULL)
	{
		priv->on_event(priv->user_data, &event);
	}

	return RET_OK;
}

static void ftk_source_primary_destroy(FtkSource* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		close(priv->read_fd);
		close(priv->write_fd);
		FTK_ZFREE(thiz, sizeof(FtkSource) + sizeof(PrivInfo));
	}

	return;
}

FtkSource* ftk_source_primary_create(FtkOnEvent on_event, void* user_data)
{
	FtkSource* thiz = (FtkSource*)FTK_ZALLOC(sizeof(FtkSource) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		int pipes[2] = {0};
		DECL_PRIV(thiz, priv);
		
		thiz->get_fd   = ftk_source_primary_get_fd;
		thiz->check    = ftk_source_primary_check;
		thiz->dispatch = ftk_source_primary_dispatch;
		thiz->destroy  = ftk_source_primary_destroy;

		pipe(pipes);
		thiz->ref = 1;
		priv->read_fd   = pipes[0];
		priv->write_fd  = pipes[1];
		priv->on_event  = on_event;
		priv->user_data = user_data;
	}

	return thiz;
}

Ret ftk_source_queue_event(FtkSource* thiz, FtkEvent* event)
{
	int ret = 0;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	ret = write(priv->write_fd, event, sizeof(FtkEvent));

	return ret == sizeof(FtkEvent) ? RET_OK : RET_FAIL;
}

