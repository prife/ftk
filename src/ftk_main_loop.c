/*
 * File: ftk_main_loop.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   main loop.
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

#include "ftk_log.h"
#include "ftk_main_loop.h"

#define MAX_SOURCES 32

struct _FtkMainLoop
{
	int read_fd;
	int write_fd;
	fd_set fdset;
	int running;
	FtkSourcesManager* sources_manager;
};

typedef enum _FtkRequestType
{
	FTK_REQUEST_QUIT = 1,
}FtkRequestType;

typedef struct _FtkRequest
{
	int   type;
	void* data;
}FtkRequest;

FtkMainLoop* ftk_main_loop_create(FtkSourcesManager* sources_manager)
{
	int pipes[2] = {0};
	FtkMainLoop* thiz = NULL;
	return_val_if_fail(sources_manager != NULL, NULL);

	thiz = (FtkMainLoop*)FTK_ZALLOC(sizeof(FtkMainLoop));
	if(thiz != NULL)
	{
		pipe(pipes);
		thiz->read_fd  = pipes[0];
		thiz->write_fd = pipes[1];
		thiz->sources_manager = sources_manager;
		FD_ZERO(&thiz->fdset);
	}

	return thiz;
}

static Ret ftk_main_loop_handle_request(FtkMainLoop* thiz)
{
	int ret = 0;
	FtkRequest request = {0};

	ret = read(thiz->read_fd, &request, sizeof(FtkRequest));
	return_val_if_fail(ret == sizeof(FtkRequest), RET_FAIL);

	switch(request.type)
	{
		case FTK_REQUEST_QUIT:
		{
			thiz->running = 0;
			break;
		}
		default:break;
	}

	return RET_OK;
}

Ret ftk_main_loop_run(FtkMainLoop* thiz)
{
	int i = 0;
	int n = 0;
	int fd = 0;
	int mfd = 0;
	int ret = 0;
	int wait_time = 3000;
	int source_wait_time = 0;
	struct timeval tv = {0};
	FtkSource* source = NULL;

	thiz->running = 1;
	while(thiz->running)
	{
		n = 0;
		FD_ZERO(&thiz->fdset);
		FD_SET(thiz->read_fd, &thiz->fdset);
		FD_SET(ftk_sources_manager_get_async_pipe(thiz->sources_manager), &thiz->fdset);
	
		mfd = thiz->read_fd > ftk_sources_manager_get_async_pipe(thiz->sources_manager) ?
			thiz->read_fd : ftk_sources_manager_get_async_pipe(thiz->sources_manager);

		for(i = 0; i < ftk_sources_manager_get_count(thiz->sources_manager); i++)
		{
			source = ftk_sources_manager_get(thiz->sources_manager, i);
			if((fd = ftk_source_get_fd(source)) >= 0)
			{
				FD_SET(fd, &thiz->fdset);
				if(mfd < fd) mfd = fd;
				n++;
			}

			source_wait_time = ftk_source_check(source);
			if(source_wait_time >= 0 && source_wait_time < wait_time)
			{
				wait_time = source_wait_time;
			}
		}

		tv.tv_sec = wait_time/1000;
		tv.tv_usec = (wait_time%1000) * 1000;
		ret = select(mfd + 1, &thiz->fdset, NULL, NULL, &tv);
		
		for(i = 0; i < ftk_sources_manager_get_count(thiz->sources_manager);)
		{
			if(ftk_sources_manager_need_refresh(thiz->sources_manager))
			{
				break;
			}

			source = ftk_sources_manager_get(thiz->sources_manager, i);

			if((fd = ftk_source_get_fd(source)) >= 0 && FD_ISSET(fd, &thiz->fdset))
			{
				if(ftk_source_dispatch(source) != RET_OK)
				{
					/*as current is removed, the next will be move to current, so dont call i++*/
					ftk_sources_manager_remove(thiz->sources_manager, source);
					ftk_logd("%s:%d remove %p\n", __func__, __LINE__, source);
				}
				else
				{
					i++;
				}
				continue;
			}

			if((source_wait_time = ftk_source_check(source)) == 0)
			{
				if(ftk_source_dispatch(source) != RET_OK)
				{
					/*as current is removed, the next will be move to current, so dont call i++*/
					ftk_sources_manager_remove(thiz->sources_manager, source);
					ftk_logd("%s:%d remove %p\n", __func__, __LINE__, source);
				}
				else
				{
					i++;
				}
				continue;
			}

			i++;
		}

		if(FD_ISSET(thiz->read_fd, &thiz->fdset))
		{
			ftk_main_loop_handle_request(thiz);
		}

		if(FD_ISSET(ftk_sources_manager_get_async_pipe(thiz->sources_manager), &thiz->fdset))
		{
			ftk_sources_manager_handle_async(thiz->sources_manager);
		}
	}

	return RET_OK;
}

Ret ftk_main_loop_quit(FtkMainLoop* thiz)
{
	int ret = 0;
	FtkRequest request = {0};
	return_val_if_fail(thiz != NULL, RET_FAIL);

	request.type = FTK_REQUEST_QUIT;
	ret = write(thiz->write_fd, &request, sizeof(FtkRequest));

	return ret == sizeof(FtkRequest) ? RET_OK : RET_FAIL;
}

Ret ftk_main_loop_add_source(FtkMainLoop* thiz, FtkSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_FAIL);

	return ftk_sources_manager_add_async(thiz->sources_manager, source);
}

Ret ftk_main_loop_remove_source(FtkMainLoop* thiz, FtkSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_FAIL);

	return ftk_sources_manager_remove_async(thiz->sources_manager, source);
}

void ftk_main_loop_destroy(FtkMainLoop* thiz)
{
	if(thiz != NULL)
	{
		close(thiz->read_fd);
		close(thiz->write_fd);
		FTK_ZFREE(thiz, sizeof(FtkMainLoop));
	}

	return;
}

