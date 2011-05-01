/*
 * File: ftk_app.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   application interface
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
 * 2010-08-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_APP_H
#define FTK_APP_H

#include "ftk.h"

FTK_BEGIN_DECLS

struct _FtkApp;
typedef struct _FtkApp FtkApp;

typedef FtkBitmap*  (*FtkAppGetIcon)(FtkApp* thiz);
typedef const char* (*FtkAppGetName)(FtkApp* thiz);
typedef Ret  (*FtkAppRun)(FtkApp* thiz, int argc, char* argv[]);
typedef void (*FtkAppDestroy)(FtkApp* thiz);
typedef FtkApp* (*FtkLoadApp)(void);

struct _FtkApp
{
	FtkAppGetIcon get_icon;
	FtkAppGetName get_name;
	FtkAppRun     run;
	FtkAppDestroy destroy;

	char priv[ZERO_LEN_ARRAY];
};

static inline FtkBitmap* ftk_app_get_icon(FtkApp* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->get_icon != NULL, NULL);

	return thiz->get_icon(thiz);
}

static inline const char* ftk_app_get_name(FtkApp* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->get_name != NULL, NULL);

	return thiz->get_name(thiz);
}

static inline Ret ftk_app_run(FtkApp* thiz, int argc, char* argv[])
{
	return_val_if_fail(thiz != NULL && thiz->run != NULL, RET_FAIL);

	return thiz->run(thiz, argc, argv);
}

static inline void ftk_app_destroy(FtkApp* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
//		thiz->destroy(thiz);
	}

	return;
}

FTK_END_DECLS

#endif/*FTK_APP_H*/

