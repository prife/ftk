/*
 * File: ftk_display_dfb.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   display directfb implementation.
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_globals.h"
#include <directfb.h>
#include "ftk_display_dfb.h"
#include "ftk_source_dfb.h"

#define DFBCHECK(x) x

typedef struct _PrivInfo
{
	int width;
	int height;
	IDirectFB* dfb;
	IDirectFBSurface* primary;
}PrivInfo;

static Ret ftk_display_dfb_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	int pitch = 0;
	void *data = NULL;
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	int display_width  = ftk_display_width(thiz);
	int display_height = ftk_display_height(thiz);
	IDirectFBSurface* surface = priv->primary;
	
	surface->Lock(surface, DSLF_READ | DSLF_WRITE, &data, &pitch);
	ret = ftk_bitmap_copy_to_data_argb(bitmap, rect, data, xoffset, yoffset, display_width, display_height);
	surface->Unlock(surface);
	surface->Flip(surface, NULL, 0);

	return ret;
}

static int ftk_display_dfb_width(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, 0);	

	return priv->width;
}

static int ftk_display_dfb_height(FtkDisplay* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, 0);	

	return priv->height;
}

static Ret ftk_display_dfb_snap(FtkDisplay* thiz, size_t x, size_t y, FtkBitmap* bitmap)
{
	int pitch = 0;
	void *data = NULL;
	FtkRect rect = {0};
	DECL_PRIV(thiz, priv);
	int w = ftk_display_width(thiz);
	int h = ftk_display_height(thiz);
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);

	IDirectFBSurface* surface = priv->primary;
	surface->Lock(surface, DSLF_READ | DSLF_WRITE, &data, &pitch);
	
	rect.x = x;
	rect.y = y;
	rect.width = bw;
	rect.height = bh;
	ftk_bitmap_copy_from_data_argb(bitmap, data, w, h, &rect);

	surface->Unlock(surface);

	return RET_OK;
}

static void ftk_display_dfb_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		if(priv->primary != NULL)
		{
			priv->primary->Release(priv->primary);
			priv->primary = NULL;
		}

		if(priv->dfb != NULL)
		{
			priv->dfb->Release( priv->dfb );
			priv->dfb = NULL;
		}

		FTK_ZFREE(thiz, sizeof(FtkDisplay) + sizeof(PrivInfo));
	}

	return;
}

static Ret ftk_display_dfb_init(FtkDisplay* thiz, IDirectFB* dfb)
{	
	DECL_PRIV(thiz, priv);
	int screen_height = 0;
	int screen_width  = 0;
	DFBSurfaceDescription  sdsc;
	IDirectFBSurface* primary = NULL;
	
	dfb->SetCooperativeLevel( dfb, DFSCL_FULLSCREEN );

	sdsc.flags = DSDESC_CAPS;
	sdsc.caps  = DSCAPS_PRIMARY;
	DFBCHECK(dfb->CreateSurface( dfb, &sdsc, &primary));

	primary->GetSize( primary, &screen_width, &screen_height );
	priv->dfb     = dfb;
	priv->primary = primary;
	priv->width   = screen_width;
	priv->height  = screen_height;

	primary->Clear(primary, 0xff, 0, 0, 0);
	primary->Flip(primary, NULL, 0);

	return RET_OK;
}

FtkDisplay* ftk_display_dfb_create(IDirectFB* dfb)
{
	FtkDisplay* thiz = FTK_ZALLOC(sizeof(FtkDisplay)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->update  = ftk_display_dfb_update;
		thiz->width   = ftk_display_dfb_width;
		thiz->height  = ftk_display_dfb_height;
		thiz->snap    = ftk_display_dfb_snap;
		thiz->destroy = ftk_display_dfb_destroy;

		ftk_display_dfb_init(thiz, dfb);
	}
	
	return thiz;
}


