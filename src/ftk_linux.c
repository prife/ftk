/*
 * File: ftk_linux.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   linux specific functions.
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
 * 2009-11-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include <signal.h>
#include "ftk_typedef.h"

static void signal_handler(int sig)
{
	int i = 0;
	void* buffer[100] = {0};
	int n = backtrace(buffer, 100);
	const char** symbols = (const char**)backtrace_symbols(buffer, n);

	for(i = 0; i < n; i++)
	{
		printf("%p: %s\n", buffer[i], symbols[i]);
	}

	fflush(stdout);

	signal(SIGABRT, NULL);
	signal(SIGSEGV, NULL);
	signal(SIGILL,  NULL);
	signal(SIGKILL, NULL);

	_exit(0);

	return;
}

void ftk_install_crash_signal(void)
{
	signal(SIGABRT, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGKILL, signal_handler);
	
	return ;
}

