
/*
 * File: ftk_linux.h
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
 * 2009-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_ARM_ADS_H
#define FTK_ARM_ADS_H

#ifdef ARM_ADS
#define FTK_FONT "./unicode.fnt"
#define DATA_DIR ""
#define LOCAL_DATA_DIR ""
#define usleep 
#define pipe(fds) _pipe(fds, 2, 0)

#include "ctype.h"
#define ftk_strdup(p) (char*)0    
#define ftk_strncpy   strncpy
#define ftk_snprintf  snprintf
#define ftk_vsnprintf vsnprintf 
#define inline 
#endif

#endif/*FTK_ARM_ADS_H*/

