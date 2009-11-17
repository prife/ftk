/*
 * File: ftk_display_win32.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   win32 display
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
 * 2009-11-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <windows.h>

char szClassName[ ] = "FtkEmulator";
LRESULT CALLBACK WinProc (HWND, UINT, WPARAM, LPARAM);

static HWND ftk_create_display(void)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = NULL;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WinProc;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = NULL;
    wincl.hIconSm = NULL;
    wincl.hCursor = NULL;
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "FtkEmulator",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           320,                 /* The programs width */
           480,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           NULL,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, SW_SHOW);

	return hwnd;
}


typedef struct _PrivInfo
{
	HWND wnd;
}PrivInfo;

static LRESULT CALLBACK WinProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

static Ret ftk_display_update(Ftkdisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	DECL_PRIV(thiz, priv);

	return RET_OK;
}

static int ftk_display_width(Ftkdisplay* thiz)
{
	RECT r = {0};
	DECL_PRIV(thiz, priv);
	GetWindowRect(priv->wnd, &r);

	return r->width;
}

static int ftk_display_height(Ftkdisplay* thiz)
{
	RECT r = {0};
	DECL_PRIV(thiz, priv);
	GetWindowRect(priv->wnd, &r);

	return r->height;
}

static int ftk_display_bitsPerPixel(Ftkdisplay* thiz)
{
	DECL_PRIV(thiz, priv);

	return 16;
}

static Ret ftk_display_snap(Ftkdisplay* thiz, FtkBitmap** bitmap)
{
	DECL_PRIV(thiz, priv);

	return RET_OK;
}

static void ftk_display_destroy(Ftkdisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	CloseWindow(priv->wnd);

	return;
}

FtkDisplay* ftk_display_win32_create(void)
{
	FtkDisplay* thiz = FTK_ZALLOC(sizeof(FtkDisplay));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		thiz->update   = ftk_display_win32_update;
		thiz->width    = ftk_display_win32_width;
		thiz->height   = ftk_display_win32_height;
		thiz->bits_per_pixel = ftk_display_win32_bits_per_pixel;
		thiz->snap     = ftk_display_win32_snap;
		thiz->destroy = ftk_display_win32_destroy;

		priv->wnd = ftk_create_window();
	}

	return thiz;
}
