/*
 * File: ftk_pipe.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   pipe
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
 * 2010-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_pipe.h"
#include "ftk_allocator.h"

struct _FtkPipe
{
	int read_fd;
	int write_fd;
};

static Ret make_sock_pipe(int pipes[2])
{
#ifdef WIN32
	win32_socketpair(pipes);
#elif PSP
	pipe(pipes);
#else
	socketpair(AF_UNIX, SOCK_STREAM, 0, pipes);	
#endif

	return RET_OK;
}

FtkPipe* ftk_pipe_create(void)
{
	int pipes[2] = {0};
	FtkPipe* thiz = FTK_ZALLOC(sizeof(FtkPipe));
	
	if(thiz != NULL)
	{
		make_sock_pipe(pipes);
		thiz->read_fd  = pipes[0];
		thiz->write_fd = pipes[1];
	}

	return thiz;
}

int   ftk_pipe_read(FtkPipe* thiz, void* buff, size_t length)
{
	return_val_if_fail(thiz != NULL && buff != NULL, -1);

#ifdef PSP
	return read(thiz->read_fd, buff, length);
#else
	return recv(thiz->read_fd, buff, length, 0);
#endif
}

int ftk_pipe_write(FtkPipe* thiz, const void* buff, size_t length)
{
	return_val_if_fail(thiz != NULL && buff != NULL, -1);

#ifdef PSP
	return write(thiz->write_fd, buff, length);
#else
	return send(thiz->write_fd, buff, length, 0);
#endif
}

int   ftk_pipe_get_read_handle(FtkPipe* thiz)
{
	return thiz != NULL ? thiz->read_fd : -1;
}

int   ftk_pipe_get_write_handle(FtkPipe* thiz)
{
	return thiz != NULL ? thiz->write_fd : -1;
}

void  ftk_pipe_destroy(FtkPipe* thiz)
{
	if(thiz != NULL)
	{
#ifdef WIN32
		closesocket(thiz->read_fd);
		closesocket(thiz->write_fd);
#else
		close(thiz->read_fd);
		close(thiz->write_fd);
#endif
		FTK_ZFREE(thiz, sizeof(*thiz));
	}

	return;
}


