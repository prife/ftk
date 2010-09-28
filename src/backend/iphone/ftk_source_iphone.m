/*
 * File: ftk_source_iphone.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: 
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
#import <UIKit/UIKit.h>
#import "ftk_log.h"
#import "ftk_source_iphone.h"

static int ftk_source_iphone_get_fd(FtkSource* thiz)
{
    return -1;
}

static int ftk_source_iphone_check(FtkSource* thiz)
{
    return 0;
}

static Ret ftk_source_iphone_dispatch(FtkSource* thiz)
{
    SInt32 result;

    if (setjmp(*jump_env()) == 0) {
        do {
            result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, TRUE);
        } while(result == kCFRunLoopRunHandledSource);

        usleep(20000);
    }
}

static void ftk_source_iphone_destroy(FtkSource* thiz)
{
    if(thiz != NULL)
    {
        FTK_ZFREE(thiz, sizeof(thiz));
    }
}

FtkSource* ftk_source_iphone_create(void)
{
    FtkSource* thiz = NULL;

    thiz = (FtkSource*)FTK_ZALLOC(sizeof(FtkSource));

    if(thiz != NULL)
    {
        thiz->get_fd = ftk_source_iphone_get_fd;
        thiz->check = ftk_source_iphone_check;
        thiz->dispatch = ftk_source_iphone_dispatch;
        thiz->destroy = ftk_source_iphone_destroy;

        thiz->ref = 1;
    }

    return thiz;
}
