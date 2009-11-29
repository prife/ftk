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
	int i = 0;
	int j = 0;
	int k = 0;
	int pitch = 0;
	void *data = NULL;
	DECL_PRIV(thiz, priv);
	int x = rect != NULL ? rect->x : 0;
	int y = rect != NULL ? rect->y : 0;
	return_val_if_fail(thiz != NULL && bitmap != NULL, RET_FAIL);
	IDirectFBSurface* surface = priv->primary;
	int display_width  = ftk_display_width(thiz);
	int display_height = ftk_display_height(thiz);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);
	int w = rect != NULL ? rect->width : bitmap_width;
	int h = rect != NULL ? rect->height : bitmap_height;

	surface->Lock(surface, DSLF_READ | DSLF_WRITE, &data, &pitch);
	FtkColor* dst = data;
	FtkColor* src = ftk_bitmap_bits(bitmap);

	return_val_if_fail(x < bitmap_width, RET_FAIL);
	return_val_if_fail(y < bitmap_height, RET_FAIL);
	return_val_if_fail(xoffset < display_width, RET_FAIL);
	return_val_if_fail(yoffset < display_height, RET_FAIL);

	w = (x + w) < bitmap_width  ? w : bitmap_width - x;
	w = (xoffset + w) < display_width  ? w : display_width  - xoffset;
	h = (y + h) < bitmap_height ? h : bitmap_height - y;
	h = (yoffset + h) < display_height ? h : display_height - yoffset;
	
	w += x;
	h += y;

	src += y * bitmap_width;
	dst += yoffset * display_width;

	for(i = y; i < h; i++)
	{
		for(j = x, k= xoffset; j < w; j++, k++)
		{
			FtkColor* pdst = dst+k;
			FtkColor* psrc = src+j;
			if(psrc->a == 0xff)
			{
				pdst->b = psrc->r;
				pdst->g = psrc->g;
				pdst->r = psrc->b;
			}
			else
			{
				FTK_ALPHA_1(psrc->r, pdst->b, psrc->a);
				FTK_ALPHA_1(psrc->b, pdst->r, psrc->a);
				FTK_ALPHA_1(psrc->g, pdst->g, psrc->a);
			}
		}
		src += bitmap_width;
		dst += display_width;
	}
	surface->Unlock(surface);
	surface->Flip(surface, NULL, 0);

	return RET_OK;
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
	int ox = 0;
	int oy = 0;
	int pitch = 0;
	void *data = NULL;
	DECL_PRIV(thiz, priv);
	int w = ftk_display_width(thiz);
	int h = ftk_display_height(thiz);
	int bw = ftk_bitmap_width(bitmap);
	int bh = ftk_bitmap_height(bitmap);
	IDirectFBSurface* surface = priv->primary;
	surface->Lock(surface, DSLF_READ | DSLF_WRITE, &data, &pitch);
	FtkColor* src = (FtkColor*)data;
	FtkColor* dst = ftk_bitmap_bits(bitmap);

	return_val_if_fail(thiz != NULL && NULL != dst, RET_FAIL);

	w = (x + bw) < w ? bw : w - x;
	h = (y + bh) < h ? bh : h - y;

	src += y * ftk_display_width(thiz) + x;
	for(oy = 0; oy < h; oy++)
	{
		for(ox =0; ox < w; ox++)
		{
			dst[ox].a = 0xff;
			dst[ox].r = src[ox].b;
			dst[ox].g = src[ox].g;
			dst[ox].b = src[ox].r;
		}
		src += ftk_display_width(thiz);
		dst += ftk_bitmap_width(bitmap);
	}

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


