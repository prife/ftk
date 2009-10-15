/*
 * File: ftk_source_timer.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   timer.
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
#include <sys/time.h>
#include "ftk_typedef.h"
#include "ftk_source_timer.h"

typedef struct _PrivInfo
{
	int interval;
	FtkTimer action;
	void* user_data;
	struct timeval next_time;
}PrivInfo;

static int ftk_source_timer_get_fd(FtkSource* thiz)
{
	return -1;
}

static int ftk_source_timer_check(FtkSource* thiz)
{
	DECL_PRIV(thiz, priv);
	struct timeval now = {0};
	gettimeofday(&now, NULL);
	
	int t = (priv->next_time.tv_sec - now.tv_sec)*1000 + (priv->next_time.tv_usec - now.tv_usec)/1000;
	if(t < 0) t = 0;

	return t;
}

static void ftk_source_timer_calc_timer(PrivInfo* priv)
{
	int sec = 0;
	int usec = 0;
	int tv_usec = 0;
	int interval = priv->interval;

	sec = interval / 1000;
	usec = (interval % 1000) * 1000;
	gettimeofday(&priv->next_time, NULL);
	tv_usec = priv->next_time.tv_usec;
	priv->next_time.tv_sec += sec;
	priv->next_time.tv_usec += usec;
	
	if(tv_usec > priv->next_time.tv_usec && tv_usec > usec)
	{
		priv->next_time.tv_sec++;
	}

	return;
}

static Ret ftk_source_timer_dispatch(FtkSource* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv->action != NULL, RET_REMOVE);

	Ret ret = priv->action(priv->user_data);
	if(ret == RET_OK)
	{
		ftk_source_timer_calc_timer(priv);
	}

	return ret;
}

static void ftk_source_timer_destroy(FtkSource* thiz)
{
	FTK_ZFREE(thiz, sizeof(FtkSource) + sizeof(PrivInfo));

	return;
}

FtkSource* ftk_source_timer_create(int interval, FtkTimer action, void* user_data)
{
	FtkSource* thiz = (FtkSource*)FTK_ZALLOC(sizeof(FtkSource) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->get_fd   = ftk_source_timer_get_fd;
		thiz->check    = ftk_source_timer_check;
		thiz->dispatch = ftk_source_timer_dispatch;
		thiz->destroy  = ftk_source_timer_destroy;

		thiz->ref = 1;
		priv->interval  = interval;
		priv->user_data = user_data;
		priv->action = action;
		ftk_source_timer_calc_timer(priv);
	}

	return thiz;
}

#ifdef FTK_SOURCE_TIMER_TEST
Ret my_action(void* user_data)
{	
	int* p = (int*)user_data;

	(*p)++;
	printf("%d\n", *p);

	return *p == 10 ? RET_REMOVE:RET_OK;
}

int main(int argc, char* argv[])
{
	int n = 0;

	FtkSource* thiz = ftk_source_timer_create(1500, my_action, &n);
	while(1)
	{
		int t = ftk_source_check(thiz);
		printf("t=%d\n", t);
		assert(t > 1000);
		assert(ftk_source_get_fd(thiz) < 0);
		usleep(t*1000);
		if(ftk_source_dispatch(thiz) == RET_REMOVE)
		{
			break;
		}
	}
	ftk_source_unref(thiz);
	return 0;
}
#endif/*FTK_SOURCE_TIMER_TEST*/

