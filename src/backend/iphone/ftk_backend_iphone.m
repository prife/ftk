/*
 * File: ftk_backend_iphone.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: init for iphone.
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
#import "ftk_display_iphone.h"
#import "ftk_source_iphone.h"

Ret ftk_backend_init(void)
{
    FtkSource* source = NULL;
    FtkDisplay* display = NULL;

    display = ftk_display_iphone_create();
    ftk_set_display(display);

    source = ftk_source_iphone_create();
    ftk_sources_manager_add(ftk_default_sources_manager(), source);

    return display != NULL ? RET_OK : RET_FAIL;
}
