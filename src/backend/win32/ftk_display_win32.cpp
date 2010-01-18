/*
 * File: ftk_display_win32_win32.c
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
#include "ftk_bitmap.h"
#include "ftk_event.h"
#include "ftk_globals.h"
#include "ftk_wnd_manager.h"
#include "ftk_display_win32.h"

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 480

static char szClassName[ ] = "FtkEmulator";
static LRESULT CALLBACK WinProc (HWND, UINT, WPARAM, LPARAM);

static HWND ftk_create_display_window(void)
{
    HWND hwnd;               /* This is the handle for our window */
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
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 64;                      /* structure or the window instance */
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
           DISPLAY_WIDTH,       /* The programs width */
           DISPLAY_HEIGHT,      /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           NULL,                /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

	return hwnd;
}


typedef struct _PrivInfo
{
	HWND wnd;
	void* bits;
	void* revert_bits;
	HBITMAP hBitmap;
	FtkEvent event;
}PrivInfo;

static LRESULT WinOnPaint(HWND hwnd)
{
	HDC dc = NULL;
	PAINTSTRUCT ps;
	HBITMAP hBitmap;
	PrivInfo* priv = NULL;
	HBITMAP hDisplay = NULL;
	
	BeginPaint(hwnd, &ps);
	priv = (PrivInfo*)GetWindowLong(hwnd, GWL_USERDATA);
	hDisplay = priv->hBitmap;
	SetMapMode(ps.hdc, MM_TEXT);
	SetWindowOrgEx(ps.hdc, 0, -DISPLAY_HEIGHT, NULL);
	SetViewportOrgEx(ps.hdc, 0, -DISPLAY_HEIGHT, NULL);
	
	MoveToEx(ps.hdc, 0, 0, NULL);
	LineTo(ps.hdc, 100, 100);
	dc = CreateCompatibleDC(ps.hdc);
	hBitmap = (HBITMAP)SelectObject(dc, hDisplay);
	BitBlt(ps.hdc, 0, 0, DISPLAY_WIDTH,	DISPLAY_HEIGHT, dc, 0, 0, SRCCOPY);
	SelectObject(dc, hBitmap);
	DeleteObject(dc);

	EndPaint(hwnd, &ps);

	return 0;
}

static LRESULT CALLBACK WinProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PrivInfo* priv = (PrivInfo*)GetWindowLong(hwnd, GWL_USERDATA);
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
		case WM_PAINT:
			{
				WinOnPaint(hwnd);
				break;
			}
		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				break;
			}
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		//case WM_MOUSEMOVE:
			{
				int xPos = LOWORD(lParam); 
				int yPos = HIWORD(lParam); 
				priv->event.u.mouse.x = xPos;
				priv->event.u.mouse.y = yPos;
				priv->event.type = message == WM_LBUTTONUP? FTK_EVT_MOUSE_UP : FTK_EVT_MOUSE_DOWN;

				break;
			}
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

	if(priv->event.type != FTK_EVT_NOP)
	{
		ftk_wnd_manager_dispatch_event(ftk_default_wnd_manager(), &priv->event);
		priv->event.type = FTK_EVT_NOP;
	}
    return 0;
}

static Ret ftk_display_win32_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	int i = 0;
	int j = 0;
	DECL_PRIV(thiz, priv);
	BITMAPINFO bmi;
	int display_width  = DISPLAY_WIDTH;
	int display_height = DISPLAY_HEIGHT;
	FtkColor* bits = (FtkColor*)priv->bits;

	memset(&bmi, 0x00, sizeof(bmi));
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = DISPLAY_WIDTH;
	bmi.bmiHeader.biHeight = DISPLAY_HEIGHT;
	bmi.bmiHeader.biPlanes = 1;

	ftk_bitmap_copy_to_data_bgra32(bitmap, rect, 
		priv->bits, xoffset, yoffset, display_width, display_height); 

	FtkColor* src = (FtkColor*)priv->bits;
	FtkColor* dst = (FtkColor*)priv->revert_bits + ((DISPLAY_HEIGHT - 1) * DISPLAY_WIDTH);
	for(j = 0; j < DISPLAY_HEIGHT; j++)
	{
		for(i = 0; i < DISPLAY_WIDTH; i++)
		{
			dst[i] = src[i];
		}
		src += DISPLAY_WIDTH;
		dst -= DISPLAY_WIDTH;
	}

	::SetDIBits(GetDC(priv->wnd), priv->hBitmap, 0, DISPLAY_HEIGHT, priv->revert_bits, &bmi, DIB_RGB_COLORS);

	InvalidateRect(priv->wnd, NULL, FALSE);

	return RET_OK;
}

static int ftk_display_win32_width(FtkDisplay* thiz)
{
	return DISPLAY_WIDTH;
}

static int ftk_display_win32_height(FtkDisplay* thiz)
{
	return DISPLAY_HEIGHT;
}

static Ret ftk_display_win32_snap(FtkDisplay* thiz, size_t x, size_t y, FtkBitmap* bitmap)
{
	DECL_PRIV(thiz, priv);

	return RET_OK;
}

static void ftk_display_win32_destroy(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	if(priv != NULL)
	{
		CloseWindow(priv->wnd);
		FTK_FREE(priv->bits);
		FTK_FREE(priv->revert_bits);
		DeleteObject(priv->hBitmap);
		FTK_FREE(thiz);
	}

	return;
}

FtkDisplay* ftk_display_win32_create(void)
{
	FtkDisplay* thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* p = NULL;
		BITMAPINFO bmi = {0};
		HBITMAP hBitmap = NULL;
		DECL_PRIV(thiz, priv);

		thiz->update   = ftk_display_win32_update;
		thiz->width    = ftk_display_win32_width;
		thiz->height   = ftk_display_win32_height;
		thiz->snap     = ftk_display_win32_snap;
		thiz->destroy  = ftk_display_win32_destroy;

		priv->wnd = ftk_create_display_window();

		memset(&bmi, 0x00, sizeof(bmi));

		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biHeight   = DISPLAY_HEIGHT;
		bmi.bmiHeader.biWidth    = DISPLAY_HEIGHT;
		bmi.bmiHeader.biSize     = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biPlanes   = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		hBitmap = CreateBitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT, 1, 32, NULL);
		priv->hBitmap = hBitmap;
		priv->bits = FTK_ZALLOC(DISPLAY_WIDTH * DISPLAY_HEIGHT * 4);
		priv->revert_bits = FTK_ZALLOC(DISPLAY_WIDTH * DISPLAY_HEIGHT * 4);

		SetWindowLong(priv->wnd, GWL_USERDATA, (LONG)priv);
		ShowWindow (priv->wnd, SW_SHOW);
		UpdateWindow(priv->wnd);
	}

	return thiz;
}

