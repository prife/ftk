/*
 * File: ftk_animator.h   
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   animator interface.
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
 * 2009-11-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_ANIMATOR_H
#define FTK_ANIMATOR_H

#include "ftk_window.h"

FTK_BEGIN_DECLS

struct _FtkAnimator;
typedef struct _FtkAnimator FtkAnimator;

typedef Ret  (*FtkAnimatorStop)(FtkAnimator* thiz);
typedef Ret  (*FtkAnimatorStart)(FtkAnimator* thiz, FtkWidget* win, int sync);
typedef Ret  (*FtkAnimatorSetParam)(FtkAnimator* thiz, int type, int start, int end, int step, int duration);
typedef void (*FtkAnimatorDestroy)(FtkAnimator* thiz);

typedef enum _FtkExpandType
{
	FTK_ANI_NONE = 0,
	FTK_ANI_TO_RIGHT,
	FTK_ANI_TO_DOWN,
	FTK_ANI_TO_UP,
	FTK_ANI_TO_EAST_SOUTH,
	FTK_ANI_TO_EAST_NORTH,
	FTK_ANI_TO_BRINK,
	FTK_ANI_ALPHA
}FtkExpandType;

typedef enum _FtkZoomType
{
	FTK_ANI_NONE_ZOOM = 0,
	FTK_ANI_ZOOM_IN,
	FTK_ANI_ZOOM_OUT
}FtkZoomType;

struct _FtkAnimator
{
	FtkAnimatorStart    start;
	FtkAnimatorStop     stop;
	FtkAnimatorSetParam set_param;
	FtkAnimatorDestroy  destroy;
	char priv[1];
};

static inline Ret  ftk_animator_stop(FtkAnimator* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->stop != NULL, RET_FAIL);

	return thiz->stop(thiz);
}

static inline Ret  ftk_animator_start(FtkAnimator* thiz, FtkWidget* win, int sync)
{
	return_val_if_fail(thiz != NULL && thiz->stop != NULL, RET_FAIL);

	return thiz->start(thiz, win, sync);
}

static inline Ret  ftk_animator_set_param(FtkAnimator* thiz, int type, int start, int end, int step, int duration)
{
	return_val_if_fail(thiz != NULL && thiz->stop != NULL, RET_FAIL);

	return thiz->set_param(thiz, type, start, end, step, duration);
}

static inline void ftk_animator_destroy(FtkAnimator* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
		thiz->destroy(thiz);
	}

	return;
}

FTK_END_DECLS

#endif/*FTK_ANIMATOR_H*/

