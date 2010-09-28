/*
 * File: ftk_iphone.h
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: iphone relative functions. 
 *
 * Copyright (c) 2009 - 2010  ngwsx2008 <ngwsx2008@126.com>
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
 * 2010-09-28 ngwsx2008 <ngwsx2008@126.com> created.
 *
 */

#ifndef FTK_IPHONE_H
#define FTK_IPHONE_H

#include <setjmp.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#define ftk_strncpy   strncpy
#define ftk_snprintf  snprintf
#define ftk_vsnprintf vsnprintf 
#define ftk_getcwd    getcwd
#define ftk_sscanf    sscanf

#define ftk_pipe_pair(fds)				pipe(fds)
#define ftk_pipe_close(fd)				close(fd)
#define ftk_pipe_recv(fd, buf, length)	read(fd, buf, length)
#define ftk_pipe_send(fd, buf, length)	write(fd, buf, length)

#ifdef HAVE_CONFIG_H
#include "config_ftk.h"
#endif

#define FTK_PATH_DELIM '/'

#if 0
#define FTK_HAS_MAIN 1
#endif

#define USE_FTK_MAIN 1

jmp_buf *jump_env(void);

#endif/*FTK_IPHONE_H*/
