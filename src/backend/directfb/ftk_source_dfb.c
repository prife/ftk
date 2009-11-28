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
	
	return priv->fd;
}

static int  ftk_source_dfb_check(FtkSource* thiz)
{
	return -1;
}

static Ret ftk_source_dfb_dispatch_input_event(FtkSource* thiz, DFBInputEvent* event)
{
	switch(event->type)
	{
		case DIET_KEYPRESS:
		{
			ftk_logd("key_code=%d\n", event->key_code);
			break;
		}
		case DIET_KEYRELEASE:
		{
			ftk_logd("key_code=%d\n", event->key_code);
			break;
		}
		case DIET_BUTTONPRESS:
		{
			ftk_logd("button=%d axisrel=%d axisabs=%d\n", event->button, event->axisrel, event->axisabs);
			break;
		}
		case DIET_BUTTONRELEASE:
		{
			ftk_logd("button=%d axisrel=%d axisabs=%d\n", event->button, event->axisrel, event->axisabs);
			break;
		}
		case DIET_AXISMOTION:
		{
			break;
		}
	}
	return RET_OK;
}
static Ret  ftk_source_dfb_dispatch(FtkSource* thiz)
{
	int i = 0;
	int nr = 0;
	DFBEvent  *event;
	DECL_PRIV(thiz, priv);
	DFBEvent   buff[10] = {0};
	int size =  read(priv->fd, buff, sizeof(buff));
	return_val_if_fail(size > 0, RET_FAIL);

	nr = size/sizeof(DFBEvent);

	for(i = 0; i < nr; i++)
	{
		event = buff+i;
		switch(event->clazz)
		{
			case DFEC_INPUT:
			{
				ftk_source_dfb_dispatch_input_event(thiz, event);
				break;
			}
			case DFEC_USER:
			{
				break;
			}
			case DFEC_WINDOW:
			{
				break;
			}
			default:break;
		}
	}

	return RET_OK;
}

static void ftk_source_dfb_destroy(FtkSource* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		close(priv->fd);
	}

	return;
}

FtkSource* ftk_source_dfb_create(IDirectFB* dfb, int fd)
{
	FtkSource* thiz = FTK_ZALLOC(sizeof(FtkSource)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->get_fd   =  ftk_source_dfb_get_fd;
		thiz->check    = ftk_source_dfb_check;
		thiz->dispatch = ftk_source_dfb_dispatch;
		thiz->destroy  = ftk_source_dfb_destroy;

		priv->fd = fd;
	}

	return thiz;
}


