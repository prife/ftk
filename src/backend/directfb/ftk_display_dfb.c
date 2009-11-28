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

#include <directfb.h>
#include "ftk_display_dfb.h"

#define DFBCHECK(x) x

typedef struct _PrivInfo
{
	IDirectFB* dfb;
	IDirectFBSurface *surface;
	IDirectFBWindow* window;
	IDirectFBDisplayLayer  *layer;
}PrivInfo;

static Ret ftk_display_dfb_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int pitch = 0;
	void *data = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->surface != NULL && bitmap != NULL, RET_FAIL);
	int display_width  = ftk_display_width(thiz);
	int display_height = ftk_display_height(thiz);
	int x = rect != NULL ? rect->x : 0;
	int y = rect != NULL ? rect->y : 0;
	int w = rect != NULL ? rect->width : ftk_bitmap_width(bitmap);
	int h = rect != NULL ? rect->height : ftk_bitmap_height(bitmap);
	int bitmap_width   = ftk_bitmap_width(bitmap);
	int bitmap_height  = ftk_bitmap_height(bitmap);
	FtkColor* src = ftk_bitmap_bits(bitmap);
	FtkColor* dst = NULL;
	IDirectFBSurface* surface = priv->surface;
	surface->Lock(surface, DSLF_READ | DSLF_WRITE, &data, &pitch);
	dst = data;
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
	priv->layer->SetBackgroundImage(priv->layer, surface );
	priv->layer->SetBackgroundMode(priv->layer, DLBM_IMAGE );

	return RET_OK;
}

static int ftk_display_dfb_width(FtkDisplay* thiz)
{
	int w = 0;
	int h = 0;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);
	
	priv->surface->GetSize(priv->surface, &w, &h);

	return w;
}

static int ftk_display_dfb_height(FtkDisplay* thiz)
{
	int w = 0;
	int h = 0;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->surface->GetSize(priv->surface, &w, &h);

	return h;
}

static int ftk_display_dfb_bits_per_pixel(FtkDisplay* thiz)
{
	return 2;
}

static Ret ftk_display_dfb_snap(FtkDisplay* thiz, size_t x, size_t y, FtkBitmap* bitmap)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	return RET_OK;
}

static void ftk_display_dfb_destroy(FtkDisplay* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		if(priv->window != NULL)
		{
			priv->window->Release(priv->window);
			priv->window = NULL;
		}

		if(priv->dfb != NULL)
		{
			priv->dfb->Release( priv->dfb );
			priv->dfb->Release( priv->dfb );
		}
	}

	return;
}

static Ret ftk_display_dfb_init(FtkDisplay* thiz, IDirectFB* dfb)
{	
	DECL_PRIV(thiz, priv);
	IDirectFBDisplayLayer  *layer;
	
	IDirectFBSurface       *surface;
	IDirectFBWindow        *window;
	DFBDisplayLayerConfig         layer_config;
	DFBGraphicsDeviceDescription  gdesc;

     dfb->GetDeviceDescription( dfb, &gdesc );

     DFBCHECK(dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer ));

     layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE );

     if (!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) &&
           (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
     {
          layer_config.flags = DLCONF_BUFFERMODE;
          layer_config.buffermode = DLBM_BACKSYSTEM;

          layer->SetConfiguration( layer, &layer_config );
     }

     layer->GetConfiguration( layer, &layer_config );
     layer->EnableCursor ( layer, 1 );
     {
          DFBSurfaceDescription desc;
          desc.flags  = DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_CAPS;
          desc.width  = layer_config.width;
          desc.height = layer_config.height;
          desc.caps   = DSCAPS_SHARED;
          DFBCHECK(dfb->CreateSurface( dfb, &desc, &surface ) );
     }

	priv->dfb = dfb;
	priv->layer = layer;
	priv->surface =surface;
	priv->window = window;

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
		thiz->bits_per_pixel = ftk_display_dfb_bits_per_pixel;
		thiz->destroy = ftk_display_dfb_destroy;

		ftk_display_dfb_init(thiz, dfb);
	}
	
	return thiz;
}


