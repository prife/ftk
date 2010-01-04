/*
 * File: ftk_display_x11.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   X11 Display.
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
 * 2009-10-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "ftk_log.h"
#include "ftk_display_x11.h"
#include "ftk_source_x11.h"

typedef struct _PrivInfo
{
	GC gc;
	int width;
	int height;
	Window win;
	Visual*  visual;
	Display* display;
	XImage*  ximage;
	int depth;
	int pixelsize;
	unsigned char* bits;
}PrivInfo;

static Ret ftk_display_x11_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);

	if(bitmap != NULL)
	{
		int display_width  = priv->width;
		int display_height = priv->height;
		ret = ftk_bitmap_copy_to_data_bgra32(bitmap, rect, priv->bits, xoffset, yoffset, display_width, display_height);
		XPutImage(priv->display, priv->win, priv->gc, priv->ximage,
			xoffset, yoffset, xoffset, yoffset, rect->width, rect->height); 
	//	ftk_logd("%s: %d %d %d %d\n", __func__, rect->x, rect->y, rect->width, rect->height);
	}
	else
	{
		XPutImage(priv->display, priv->win, priv->gc, priv->ximage, 0, 0, 0, 0, priv->width, priv->height);
		ret = RET_OK;
	}

	XFlush(priv->display);
	XSync(priv->display, 0);

	return ret;
}

static int ftk_display_x11_width(FtkDisplay* thiz)
{
	PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL;
	return_val_if_fail(priv != NULL, 0);

	return priv->width;
}

static int ftk_display_x11_height(FtkDisplay* thiz)
{
	PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL;
	return_val_if_fail(priv != NULL, 0);

	return priv->height;
}

static Ret ftk_display_x11_snap(FtkDisplay* thiz, size_t x, size_t y, FtkBitmap* bitmap)
{
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	int w = ftk_display_width(thiz);
	int h = ftk_display_height(thiz);
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	
	rect.x = x;
	rect.y = y;
	rect.width = bw;
	rect.height = bh;

	return ftk_bitmap_copy_from_data_bgra32(bitmap, priv->bits, w, h, &rect);
}

static void ftk_display_x11_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

//		XDestroyImage(priv->ximage);
		XDestroyWindow(priv->display, priv->win);
		XFreeGC(priv->display, priv->gc);
		//XCloseDisplay(priv->display);
		FTK_FREE(priv->bits);
		FTK_ZFREE(thiz, sizeof(FtkDisplay) + sizeof(PrivInfo));
	}

	return;
}

FtkDisplay* ftk_display_x11_create(FtkSource** event_source, FtkOnEvent on_event, void* ctx)
{
	GC gc;
	int width  = 0;
	int height = 0;
	int screen = 0;
	Window win = 0;
	Display* display = XOpenDisplay(NULL);
	return_val_if_fail(display != NULL, NULL);
	screen = DefaultScreen(display);
	sscanf(DISPLAY_PARAM, "linux-x11:%dx%d", &width, &height);

	ftk_logd("%s width=%d height=%d\n", __func__, width, height);
	win = XCreateSimpleWindow(display, RootWindow(display, screen),
		0, 0, width, height, 3, BlackPixel(display, screen), WhitePixel(display, screen));
	XSelectInput(display, win, ExposureMask|KeyPressMask |KeyReleaseMask| ButtonPressMask 
		|ButtonReleaseMask|StructureNotifyMask | PointerMotionMask);
	gc = XCreateGC(display, win, 0, NULL);
	XMapWindow(display, win);

	XEvent event = {0};
	XNextEvent(display, &event);
	FtkDisplay* thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));
	
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		thiz->update   = ftk_display_x11_update;
		thiz->snap     = ftk_display_x11_snap;
		thiz->width	   = ftk_display_x11_width;
		thiz->height   = ftk_display_x11_height;
		thiz->destroy  = ftk_display_x11_destroy;
	
		priv->gc      = gc;
		priv->win     = win;
		priv->width   = width;
		priv->height  = height;
		priv->display = display;
		priv->visual  = DefaultVisualOfScreen(DefaultScreenOfDisplay(display));
		priv->depth   = DefaultDepth(display, screen);
		
		if(priv->depth == 8) priv->pixelsize = 1;
		else if(priv->depth == 16) priv->pixelsize = 2;
		else priv->pixelsize = 4;

		assert(priv->pixelsize == 4);
		priv->bits = calloc(1, width * height * priv->pixelsize);
		memset(priv->bits, 0xff, width * height * priv->pixelsize);
		/*FIXME: force to 32bit*/
		priv->ximage = XCreateImage(display, priv->visual, priv->depth, ZPixmap,
			0, (char*)priv->bits, width, height,
			32, width * priv->pixelsize);

		*event_source = ftk_source_x11_create(thiz, on_event, ctx);

		ftk_logd("%s: pixelsize=%d\n", __func__, priv->pixelsize);
	}

	return thiz;
}

Ret ftk_display_x11_on_resize(FtkDisplay* thiz, int width, int height)
{
	PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL;
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->width  = width;
	priv->height = height;
	ftk_display_x11_update(thiz, NULL, NULL, 0, 0);

	return RET_OK;
}

void* ftk_display_x11_get_xdisplay(FtkDisplay* thiz)
{
	PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL;
	return_val_if_fail(priv != NULL, NULL);

	return (void*)priv->display;
}

void* ftk_display_x11_get_xwindow(FtkDisplay* thiz)
{
	PrivInfo* priv = thiz != NULL ? (PrivInfo*)thiz->priv : NULL;
	return_val_if_fail(priv != NULL, NULL);

	return (void*)priv->win;
}


