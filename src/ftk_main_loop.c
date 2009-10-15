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
#include "ftk_main_loop.h"

#define MAX_SOURCES 32

struct _FtkMainLoop
{
	int read_fd;
	int write_fd;
	fd_set fdset;
	int running;
	FtkSource* sources[MAX_SOURCES];
};

typedef enum _FtkRequestType
{
	FTK_REQUEST_QUIT = 1,
	FTK_REQUEST_ADD_SOURCE,
	FTK_REQUEST_REMOVE_SOURCE,
}FtkRequestType;

typedef struct _FtkRequest
{
	int   type;
	void* data;
	void* data1;
	void* data2;
}FtkRequest;

FtkMainLoop* ftk_main_loop_create()
{
	int pipes[2] = {0};
	FtkMainLoop* thiz = (FtkMainLoop*)FTK_ZALLOC(sizeof(FtkMainLoop));
	
	if(thiz != NULL)
	{
		pipe(pipes);
		thiz->read_fd  = pipes[0];
		thiz->write_fd = pipes[1];
		FD_ZERO(&thiz->fdset);
	}

	return thiz;
}

static Ret ftk_main_loop_handle_request(FtkMainLoop* thiz)
{
	int i = 0;
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
		case FTK_REQUEST_ADD_SOURCE:
		{
			for(i = 0; i < MAX_SOURCES; i++)
			{
				if(thiz->sources[i] == NULL)
				{
					thiz->sources[i] = (FtkSource*)request.data;
					break;
				}
			}
			break;
		}
		case FTK_REQUEST_REMOVE_SOURCE:
		{
			for(i = 0; i < MAX_SOURCES; i++)
			{
				if(thiz->sources[i] == request.data)
				{
					ftk_source_unref(thiz->sources[i]);
					thiz->sources[i] = NULL;
					break;
				}
			}
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

		mfd = thiz->read_fd;
		for(i = 0; i < MAX_SOURCES; i++)
		{
			if((source = thiz->sources[i]) == NULL)
			{
				continue;
			}

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
		
		for(i = 0; i < MAX_SOURCES; i++)
		{
			if((source = thiz->sources[i]) == NULL)
			{
				continue;
			}

			if((fd = ftk_source_get_fd(source)) >= 0 && FD_ISSET(fd, &thiz->fdset))
			{
				if(ftk_source_dispatch(source) != RET_OK)
				{
					thiz->sources[i] = NULL;
					ftk_source_unref(source);
					printf("%s:%d remove %p\n", __func__, __LINE__, source);
				}
				continue;
			}

			if((source_wait_time = ftk_source_check(source)) == 0)
			{
				if(ftk_source_dispatch(source) != RET_OK)
				{
					thiz->sources[i] = NULL;
					ftk_source_unref(source);
					printf("%s:%d remove %p\n", __func__, __LINE__, source);
				}
				continue;
			}
		}

		if(FD_ISSET(thiz->read_fd, &thiz->fdset))
		{
			ftk_main_loop_handle_request(thiz);
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
	int ret = 0;
	FtkRequest request = {0};
	return_val_if_fail(thiz != NULL && source != NULL, RET_FAIL);

	request.data = source;
	request.type = FTK_REQUEST_ADD_SOURCE;
	ret = write(thiz->write_fd, &request, sizeof(FtkRequest));

	return ret == sizeof(FtkRequest) ? RET_OK : RET_FAIL;
}

Ret ftk_main_loop_remove_source(FtkMainLoop* thiz, FtkSource* source)
{
	int ret = 0;
	FtkRequest request = {0};
	return_val_if_fail(thiz != NULL && source != NULL, RET_FAIL);

	request.data = source;
	request.type = FTK_REQUEST_REMOVE_SOURCE;
	ret = write(thiz->write_fd, &request, sizeof(FtkRequest));

	return ret == sizeof(FtkRequest) ? RET_OK : RET_FAIL;
}

void ftk_main_loop_destroy(FtkMainLoop* thiz)
{
	int i = 0;
	FtkSource* source = NULL;

	if(thiz != NULL)
	{
		for(i = 0; i < MAX_SOURCES; i++)
		{
			if((source = thiz->sources[i]) != NULL)
			{
				ftk_source_unref(source);
			}
		}

		close(thiz->read_fd);
		close(thiz->write_fd);
		FTK_ZFREE(thiz, sizeof(FtkMainLoop));
	}

	return;
}

