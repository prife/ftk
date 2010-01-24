/*
 * File: ftk_typedef.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:  common used type definition. 
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_TYPEDEF_H
#define FTK_TYPEDEF_H

#include "ftk_platform.h"

typedef enum _FtkBool
{
	FTK_TRUE = 1,
	FTK_FALSE = 0
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
	unsigned char b;
	unsigned char g;
	unsigned char r;
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
	RET_IGNORED,
	RET_NO_TARGET,
	RET_OUT_OF_SPACE
}Ret;

typedef enum _FtkWidgetType
{
	FTK_WIDGET_NONE   = 0,
	FTK_WINDOW = 1,
	FTK_DIALOG,
	FTK_STATUS_PANEL,
	FTK_MENU_PANEL,
	FTK_MENU_ITEM,
	FTK_LABEL,
	FTK_BUTTON,
	FTK_IMAGE,
	FTK_ENTRY,
	FTK_PAINTER,
	FTK_WAIT_BOX,
	FTK_STATUS_ITEM,
	FTK_PROGRESS_BAR,
	FTK_RADIO_GROUP,
	FTK_RADIO_BUTTON,
	FTK_CHECK_BUTTON,
	FTK_SCROLL_VBAR,
	FTK_SCROLL_HBAR,
	FTK_LIST_VIEW,
	FTK_ICON_VIEW,
	FTK_TEXT_VIEW,
	FTK_WIDGET_TYPE_NR
}FtkWidgetType;

typedef enum _FtkLogLevel
{
	FTK_LOG_V = 0,
	FTK_LOG_D = 1,
	FTK_LOG_E = 2
}FtkLogLevel;

typedef enum _FtkWidgetState
{
	FTK_WIDGET_NORMAL      = 0,
	FTK_WIDGET_FOCUSED     = 1,
	FTK_WIDGET_ACTIVE      = 2,
	FTK_WIDGET_INSENSITIVE = 3,
	FTK_WIDGET_STATE_NR    = 4
}FtkWidgetState;

typedef enum _FtkWidgetAttr
{
	FTK_ATTR_TRANSPARENT    = 1,
	FTK_ATTR_NO_FOCUS       = 2,
	FTK_ATTR_BG_CENTER      = 4,
	FTK_ATTR_BG_TILE        = 8,
	FTK_ATTR_BG_FOUR_CORNER = 16,
	FTK_ATTR_INSENSITIVE    = 32,
	FTK_ATTR_FOCUSED        = 64,
	FTK_ATTR_IGNORE_CLOSE   = 128 /*for window only*/
}FtkWidgetAttr;

typedef void (*FtkDestroy)(void* user_data);
typedef Ret (*FtkIdle)(void* user_data);
typedef Ret (*FtkTimer)(void* user_data);
typedef Ret (*FtkListener)(void* user_data, void* obj);

#define FTK_INHERITE_FROM(parent) extern int a;

#define FTK_CALL_LISTENER(listener, u, o) listener != NULL ? listener(u, o) : RET_OK
#define return_if_fail(p) if(!(p)) { printf("%s:%d "#p" failed.\n", __func__, __LINE__); return;}
#define return_val_if_fail(p, val) if(!(p)) {printf("%s:%d "#p" failed.\n", __func__, __LINE__); return (val);}
#define DECL_PRIV(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL
#define DECL_PRIV0(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv_subclass[0] : NULL
#define DECL_PRIV1(thiz, priv) PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv_subclass[1] : NULL

#define FTK_HALF(a)    ((a)>>1)
#define FTK_MIN(a, b) (a) < (b) ? (a) : (b)
#define FTK_MAX(a, b) (a) < (b) ? (b) : (a)

#define FTK_MASK_BITS(val32, index) (((val32) << ((index)%32)) & 0x80000000)

#define FTK_ALPHA_1(s, d, a) (d) = ((unsigned int)((d) * (0xff - (a)) + (s) * (a)) + 0xff) >> 8
#define FTK_ALPHA(sc, dc, a) FTK_ALPHA_1(sc->r, dc->r, a); \
	     FTK_ALPHA_1(sc->g, dc->g, a); \
	     FTK_ALPHA_1(sc->b, dc->b, a);

/*for customize*/
#define FTK_H_MARGIN            2
#define FTK_V_MARGIN            2
#define FTK_MAX_PATH            260
#define FTK_MAX_WINDOWS         32
#define FTK_SPACE_WIDTH         5
#define FTK_MAX_IMAGE_DECODERS  6
#define FTK_STATUS_PANEL_HEIGHT 36
#define FTK_DIALOG_MARGIN       6
#define FTK_DIALOG_BORDER       3
#define FTK_DIALOG_TITLE_HEIGHT 32
#define FTK_SCROLL_BAR_WIDTH    10
#define FTK_MENU_MAX_ITEM       16
#define FTK_MENU_ITEM_HEIGHT    48
#define FTK_MENU_ITEM_WIDTH     100
#define FTK_STOCK_IMG_SUFFIX    ".png"
#define FTK_FONT_SIZE 16
#define FTK_LABEL_LEFT_MARGIN 3
#define FTK_LABEL_TOP_MARGIN  1

#ifdef __cplusplus
#define FTK_BEGIN_DECLS extern "C" {
#define FTK_END_DECLS }
#else
#define FTK_BEGIN_DECLS
#define FTK_END_DECLS
#endif

#ifdef FTK_HAS_MAIN
#define FTK_MAIN main
#else
#define FTK_MAIN ftk_main
#endif

#endif/*FTK_TYPEDEF_H*/

