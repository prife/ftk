/*
 * File: ftk_wnd_manager.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   a simple window manager. 
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

#include "ftk_globals.h"
#include "ftk_wnd_manager.h"
#include "ftk_source_primary.h"

#define FTK_MAX_GLOBAL_LISTENER 12

struct _FtkWndManager
{
	int top;
	FtkMainLoop* main_loop;
	FtkSource* primary_source;
	FtkWidget* windows[FTK_MAX_WINDOWS];
	FtkWidget* grab_widget;

	void* global_listeners_ctx[FTK_MAX_GLOBAL_LISTENER];
	FtkListener global_listeners[FTK_MAX_GLOBAL_LISTENER];
};

FtkWndManager* ftk_wnd_manager_create(FtkMainLoop* main_loop)
{
	FtkWndManager* thiz = NULL;
	return_val_if_fail(main_loop != NULL, NULL);
	return_val_if_fail(thiz == NULL, NULL);

	if((thiz = (FtkWndManager*)FTK_ZALLOC(sizeof(FtkWndManager))) != NULL)
	{
		thiz->main_loop = main_loop;
		thiz->primary_source = ftk_source_primary_create(
			(FtkOnEvent)ftk_wnd_manager_dispatch, thiz);
		ftk_sources_manager_add(ftk_default_sources_manager(), thiz->primary_source);
	}

	return thiz;
}

Ret  ftk_wnd_manager_grab(FtkWndManager* thiz, FtkWidget* window)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz->grab_widget = window;

	return RET_OK;
}

Ret  ftk_wnd_manager_add(FtkWndManager* thiz, FtkWidget* window)
{
	return_val_if_fail(thiz != NULL && window != NULL, RET_FAIL);
	return_val_if_fail((thiz->top+1) < FTK_MAX_WINDOWS, RET_FAIL);

	thiz->windows[thiz->top++] = window;

	return RET_OK;
}

Ret  ftk_wnd_manager_remove(FtkWndManager* thiz, FtkWidget* window)
{
	int i = 0;
	return_val_if_fail(thiz != NULL && window != NULL, RET_FAIL);

	if(thiz->grab_widget == window)
	{
		thiz->grab_widget = NULL;
	}

	if(thiz->windows[thiz->top] == window)
	{
		thiz->top--;
		thiz->windows[thiz->top] = NULL;
	}
	else
	{
		for(i = 0; i < thiz->top; i++)
		{
			if(thiz->windows[i] == window)
			{
				for(; i < thiz->top; i++)
				{
					thiz->windows[i] = thiz->windows[i+1];
				}
				thiz->top--;
				break;
			}
		}
	}

	if(thiz->top > 0)
	{
		ftk_widget_paint(thiz->windows[thiz->top]);
	}

	return RET_OK;
}

Ret  ftk_wnd_manager_dispatch(FtkWndManager* thiz, FtkEvent* event)
{
	int i = 0;
	Ret ret = RET_OK;
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	for(i = 0; i < FTK_MAX_GLOBAL_LISTENER; i++)
	{
		if(thiz->global_listeners[i] != NULL)
		{
			ret = thiz->global_listeners[i](thiz->global_listeners_ctx[i], event);
			if(ret == RET_REMOVE)
			{
				printf("%s: event is filtered by global listeners\n", __func__);
				return ret;
			}
		}
	}

	if(event->type == FTK_EVT_WND_DESTROY)
	{
		ftk_wnd_manager_remove(thiz, event->widget);

		return RET_OK;
	}

	if(event->widget != NULL)
	{
		return ftk_widget_event((FtkWidget*)event->widget, event);
	}
	else if(thiz->grab_widget != NULL)
	{
		return ftk_widget_event(thiz->grab_widget, event);
	}
#if 0
	else if(event->type == FTK_EVT_MOUSE_DOWN || event->type == FTK_EVT_MOUSE_UP
		|| event->type == FTK_EVT_MOUSE_MOVE)
	{
		int x = event->u.mouse.x;
		int y = event->u.mouse.y;

		int i = thiz->top;
		for(; i > 0; i--)
		{
			FtkWidget* win = thiz->windows[i-1];

			if(ftk_widget_is_insensitive(win) || !ftk_widget_is_visible(win))
			{
				continue;
			}
			
			if(x >= ftk_widget_left_abs(win) && y >= ftk_widget_top_abs(win)
				&& x < (ftk_widget_left_abs(win)  + ftk_widget_width(win))
				&& y < (ftk_widget_top_abs(win) + ftk_widget_height(win)))
			{
				if(event->type == FTK_EVT_MOUSE_DOWN)
				{
					ftk_wnd_manager_grab(thiz, win);
				}
				ftk_widget_event(win, event);
				break;
			}
		}
	}
	else if(event->type == FTK_EVT_MOUSE_DOWN || event->type == FTK_EVT_MOUSE_UP)
	{
		if(thiz->top > 0)
		{
			ftk_widget_event(thiz->windows[thiz->top - 1], event);
		}
	}
#else
	else if(thiz->top > 0)
	{
		/*always send to the top window.*/
		ftk_widget_event(thiz->windows[thiz->top - 1], event);
	}
#endif
	return RET_OK;
}

Ret  ftk_wnd_manager_queue_event(FtkWndManager* thiz, FtkEvent* event)
{
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	return ftk_source_queue_event(thiz->primary_source, event);
}

Ret  ftk_wnd_manager_add_global_listener(FtkWndManager* thiz, FtkListener listener, void* ctx)
{
	int i = 0;
	return_val_if_fail(thiz != NULL && listener != NULL, RET_FAIL);

	for(i = 0; i < FTK_MAX_GLOBAL_LISTENER; i++)
	{
		if(thiz->global_listeners[i] == NULL)
		{
			thiz->global_listeners[i] = listener;
			thiz->global_listeners_ctx[i] = ctx;

			return RET_OK;
		}
	}

	return RET_OUT_OF_SPACE;
}

Ret  ftk_wnd_manager_remove_global_listener(FtkWndManager* thiz, FtkListener listener, void* ctx)
{
	int i = 0;
	return_val_if_fail(thiz != NULL && listener != NULL, RET_FAIL);

	for(i = 0; i < FTK_MAX_GLOBAL_LISTENER; i++)
	{
		if(thiz->global_listeners[i] == listener && thiz->global_listeners_ctx[i] == ctx)
		{
			thiz->global_listeners[i] = NULL;
			thiz->global_listeners_ctx[i] = NULL;
		}
	}

	return RET_OK;
}

void ftk_wnd_manager_destroy(FtkWndManager* thiz)
{
	int i = 0;
	if(thiz != NULL)
	{
		for(i = 0; i < thiz->top; i++)
		{
			ftk_widget_unref(thiz->windows[i]);
			thiz->windows[i] = NULL;
		}

		FTK_ZFREE(thiz, sizeof(FtkWndManager));
	}

	return;
}

#ifdef FTK_WND_MANAGER_TEST
int main(int argc, char* argv[])
{
	FtkWndManager* thiz = NULL;
	FtkMainLoop* main_loop = ftk_main_loop_create();
	
	thiz = ftk_wnd_manager_create(main_loop);

	ftk_wnd_manager_destroy(thiz);

	return 0;
}
#endif/*FTK_WND_MANAGER_TEST*/

