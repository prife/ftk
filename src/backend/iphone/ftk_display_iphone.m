/*
 * File: ftk_display_iphone.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: display implememtation for iphone.
 *
 * Copyright (c) 2009 - 2010  ngwsx2008 <ngwsx2008@126.com>
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
 * 2010-09-28 ngwsx2008 <ngwsx2008@126.com> created.
 *
 */

#import "ftk_display_iphone.h"

static Ret ftk_display_iphone_update(FtkDisplay* thiz, FtkBitmap* bitmap, FtkRect* rect, int xoffset, int yoffset)
{
    Ret ret = RET_OK;
    DECL_PRIV(thiz, priv);

#if 0
    ftk_logd("==============================\n");
    ftk_logd("ftk_display_iphone_update() bitmap->width:%d bitmap->height:%d "
        "rect->x:%d rect->y:%d rect->width:%d rect->height:%d xoffset:%d yoffset:%d\n",
        ftk_bitmap_width(bitmap), ftk_bitmap_height(bitmap),
        rect->x, rect->y, rect->width, rect->height, xoffset, yoffset);
    ftk_logd("==============================\n");
#endif

    ret = priv->copy_to_data(bitmap, rect,
        priv->bits, xoffset, yoffset, priv->width, priv->height);

    [priv->view drawView];
    // [priv->view setNeedsDisplay];
    [priv->view setNeedsDisplayInRect:CGRectMake(xoffset, yoffset, rect->width, rect->height)];

    return ret;
}

static int ftk_display_iphone_width(FtkDisplay* thiz)
{
    DECL_PRIV(thiz, priv);
    return_val_if_fail(priv != NULL, 0);

    return priv->width;
}


static int ftk_display_iphone_height(FtkDisplay* thiz)
{
    DECL_PRIV(thiz, priv);
    return_val_if_fail(priv != NULL, 0);

    return priv->height;
}


static Ret ftk_display_iphone_snap(FtkDisplay* thiz, FtkRect* r, FtkBitmap* bitmap)
{
    FtkRect rect = {0};
    DECL_PRIV(thiz, priv);
    int w = ftk_display_width(thiz);
    int h = ftk_display_height(thiz);
    int bw = ftk_bitmap_width(bitmap);
    int bh = ftk_bitmap_height(bitmap);

    rect.x = r->x;
    rect.y = r->y;
    rect.width = FTK_MIN(bw, r->width);
    rect.height = FTK_MIN(bh, r->height);

    return priv->copy_from_data(bitmap, priv->bits, w, h, &rect);
}


static void ftk_display_iphone_destroy(FtkDisplay* thiz)
{
    if(thiz != NULL)
    {
        DECL_PRIV(thiz, priv);

        [priv->view removeFromSuperview];
        [priv->view release];
        [priv->window release];

        FTK_FREE(priv->bits);
        FTK_ZFREE(thiz, sizeof(FtkDisplay) + sizeof(PrivInfo));
    }
}


FtkDisplay* ftk_display_iphone_create(void)
{
    FtkDisplay* thiz = (FtkDisplay*)FTK_ZALLOC(sizeof(FtkDisplay) + sizeof(PrivInfo));

    if(thiz != NULL)
    {
        DECL_PRIV(thiz, priv);
        thiz->update = ftk_display_iphone_update;
        thiz->snap = ftk_display_iphone_snap;
        thiz->width = ftk_display_iphone_width;
        thiz->height = ftk_display_iphone_height;
        thiz->destroy = ftk_display_iphone_destroy;

#if 1
        priv->depth = 24;
#else
        priv->depth = 16;
#endif

        if(priv->depth >= 24) 
        {
            priv->pixel_size = 4;
        }
        else if(priv->depth > 8)
        {
            priv->pixel_size = 2; /* 15, 16 */
        }
        else
        {
            priv->pixel_size = 1; /* 1, 2, 4, 8 */
        }

        ftk_logd("%s: pixelsize=%d, depth=%d\n", __func__, priv->pixel_size, priv->depth);

        if(priv->pixel_size == 2)
        {
            priv->copy_to_data = ftk_bitmap_copy_to_data_rgb565;
            priv->copy_from_data = ftk_bitmap_copy_from_data_rgb565;
        }
        else if(priv->pixel_size == 4)
        {
            priv->copy_to_data = ftk_bitmap_copy_to_data_bgra32;
            priv->copy_from_data = ftk_bitmap_copy_from_data_bgra32;
        }
        else
        {
            assert(!"not supported depth.");
        }

	CGRect rect = [[UIScreen mainScreen] bounds];
        priv->width   = rect.size.width;
        priv->height  = rect.size.height;

        ftk_logd("%s: width=%d, height=%d\n", __func__, priv->width, priv->height);

        priv->bits = FTK_ZALLOC(priv->width * priv->height * priv->pixel_size);

        [UIApplication sharedApplication].statusBarHidden = YES;

        priv->window = [[UIWindow alloc] initWithFrame:rect];

        priv->view = [[FtkView alloc] initWithFrame:rect];
        priv->view.display = thiz;
        [priv->view setupView];
        [priv->window addSubview:priv->view];

        [priv->window makeKeyAndVisible];
    }

    return thiz;
}
