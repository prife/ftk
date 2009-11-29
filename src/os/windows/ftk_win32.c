/*
 * File: ftk_win32.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   win32 specific functions.
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
 * 2009-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include <string.h>
#include "ftk_win32.h"

int ftk_platform_init(int argc, char** argv)
{

	return 0;
}

void ftk_platform_deinit(void)
{

	return;
}

char* ftk_strncpy(char *dest, const char *src, size_t n)
{
	return strncpy(dest, src, n);
}

char* ftk_strdup(const char *s)
{
	return strdup(s);
}

int   ftk_snprintf(char *str, size_t size, const char *format, ...)
{
	return 0;
}

int   ftk_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	return 0;
}

#define EPOCHFILETIME 0
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME        ft;
    LARGE_INTEGER   li;
    __int64         t;
    static int      tzflag;

    if (tv)
    {
        GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;       /* In 100-nanosecond intervals */
        t -= EPOCHFILETIME;     /* Offset to the Epoch time */
        t /= 10;                /* In microseconds */
        tv->tv_sec  = (long)(t / 1000000);
        tv->tv_usec = (long)(t % 1000000);
    }

    if (tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}




