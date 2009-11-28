/*
 * File: ftk_source_dfb.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   source to handle directfb event.
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_event.h"
#include "ftk_source_dfb.h"

typedef struct _PrivInfo
{
	int fd;
	IDirectFB* dfb;
	IDirectFBEventBuffer   *event_buffer;
}PrivInfo;

static int  ftk_source_dfb_get_fd(FtkSource* thiz)
{
	DECL_PRIV(thiz, priv);

	printf("%s\n", __func__);
	return priv->fd;
}

static int  ftk_source_dfb_check(FtkSource* thiz)
{
	return -1;
}

static Ret  ftk_source_dfb_dispatch(FtkSource* thiz)
{
	int i = 0;
	int nr = 0;
	DFBEvent  *event;
	DFBEvent   buff[10] = {0};
	DECL_PRIV(thiz, priv);
	int size =  read(priv->fd, buff, sizeof(buff));
	printf("%s\n", __func__);
	return_val_if_fail(size <= 0, RET_FAIL);

	return RET_OK;
}

static void ftk_source_dfb_destroy(FtkSource* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		if(priv->event_buffer != NULL)
		{
			close(priv->fd);
			priv->event_buffer->Release(priv->event_buffer);
			priv->event_buffer = NULL;
		}
	}

	return;
}

static Ret ftk_source_dfb_init(FtkSource* thiz, IDirectFB* dfb)
{
	IDirectFBEventBuffer   *event_buffer = NULL;

	DECL_PRIV(thiz, priv);

	dfb->CreateEventBuffer(dfb, &event_buffer);
	return_val_if_fail(event_buffer != NULL, RET_FAIL);

	event_buffer->CreateFileDescriptor(event_buffer, &priv->fd);
	return_val_if_fail(priv->fd > 0, RET_FAIL);

	priv->dfb = dfb;
	priv->event_buffer = event_buffer;

	return RET_OK;
}

FtkSource* ftk_source_dfb_create(IDirectFB* dfb)
{
	FtkSource* thiz = FTK_ZALLOC(sizeof(FtkSource)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->get_fd =  ftk_source_dfb_get_fd;
		thiz->check = ftk_source_dfb_check;
		thiz->dispatch = ftk_source_dfb_dispatch;
		thiz->destroy = ftk_source_dfb_destroy;

		if(ftk_source_dfb_init(thiz, dfb) != RET_OK)
		{
			FTK_ZFREE(thiz, sizeof(FtkSource)+sizeof(PrivInfo));
		}
	}

	return thiz;
}


