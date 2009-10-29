/*
 * File: ftk_typedef.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:  common used type definition. 
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

#ifndef FTK_TYPEDEF_H
#define FTK_TYPEDEF_H

#include "ftk_platform.h"

typedef enum _FtkBool
{
	FTK_TRUE = 1,
	FTK_FALSE = 0,
}FtkBool;

typedef struct _FtkPoint
{
	int x;
	int y;
}FtkPoint;

typedef struct _FtkRect
{
	int x;
	int y;
	int width;
	int height;
}FtkRect;

typedef struct _FtkColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}FtkColor;

typedef enum _Ret
{
	RET_OK,
	RET_FAIL,
	RET_REMOVE,
	RET_CONTINUE,
	RET_FOUND,
	RET_NEXT,
	RET_QUIT,
	RET_OUT_OF_SPACE
}Ret;

typedef enum _FtkWidgetType
{
	FTK_WINDOW = 1,
	FTK_DIALOG,
	FTK_PANEL,
	FTK_MENU,
	FTK_LABEL,
	FTK_BUTTON,
	FTK_IMAGE,
	FTK_EDIT,
	FTK_STATUS_ITEM
}FtkWidgetType;

typedef enum _FtkLogLevel
{
	FTK_LOG_V = 0,
	FTK_LOG_D = 1,
	FTK_LOG_E = 2,
}FtkLogLevel;

typedef void (*FtkDestroy)(void* user_data);
typedef Ret (*FtkIdle)(void* user_data);
typedef Ret (*FtkTimer)(void* user_data);
typedef Ret (*FtkListener)(void* user_data, void* obj);

#define return_if_fail(p) if(!(p)) { printf("%s:%d "#p" failed.\n", __func__, __LINE__); return;}
#define return_val_if_fail(p, val) if(!(p)) {printf("%s:%d "#p" failed.\n", __func__, __LINE__); return (val);}

#define MIN(x1, x2) ((x1) < (x2) ? (x1) : (x2))
#define MAX(x1, x2) ((x1) > (x2) ? (x1) : (x2))
#define FTK_MAX_WINDOWS 32
#define FTK_FOCUS_COLOR(c) {c.r = 0xff; c.g = 0x80; c.b = 0x80; c.a = 0xff;};
#define FTK_P_IN_R(x, y, r) (((x) >= (r)->x && (x)<((r)->x+(r)->width)) && ((y) >= (r)->y && (y) < ((r)->y + (r)->height)))

#define FTK_SPACE_WIDTH        5
#define FTK_MAX_IMAGE_DECODERS 8
#define FTK_STATUS_PANEL_HEIGHT 36

#define DECL_PRIV(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL
#define DECL_PRIV0(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv_subclass[0] : NULL
#define DECL_PRIV1(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv_subclass[1] : NULL

#define FTK_MASK_BITS(val32, index) (((val32) << ((index)%32)) & 0x80000000)

#define FTK_INHERITE_FROM(parent) /*used as comment*/

#endif/*FTK_TYPEDEF_H*/

