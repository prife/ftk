/*
 * File: ftk_image_iphone_decoder.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief:  image decoder implementation for iphone. 
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

#include <UIKit/UIKit.h>
#include "ftk_image_iphone_decoder.h"

static Ret ftk_image_iphone_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
    return_val_if_fail(filename != NULL, RET_FAIL);

    return (strstr(filename, ".bmp") != NULL
        || strstr(filename, ".png") != NULL
        || strstr(filename, ".jpg") != NULL
        || strstr(filename, ".jpeg") != NULL) ? RET_OK : RET_FAIL;
}

static FtkBitmap* load_image(const char *filename)
{
	FtkColor bg = {0};
	FtkBitmap* bitmap = NULL;
	int             height, width;
	UIImage        *img;
	CGContextRef    ctx = NULL;
	CGColorSpaceRef color_space;
	NSString       *path;

	ftk_logd("%s, %s: %s\n", __FILE__, __func__, filename);

	path = [[NSString alloc] initWithUTF8String:filename];
	img = [UIImage imageWithContentsOfFile:path];
	if (img == NULL)
	{
		[path release];
		return NULL;
	}

	width = CGImageGetWidth (img.CGImage);
	height = CGImageGetHeight (img.CGImage);

	bg.a = 0xff;
	bitmap = ftk_bitmap_create(width, height, bg);

	color_space = CGColorSpaceCreateDeviceRGB();
	ctx = CGBitmapContextCreate(ftk_bitmap_bits(bitmap),
              width, height, 8, width * 4, color_space,
              kCGImageAlphaPremultipliedLast);

	CGRect rect = CGRectMake(0, 0, width, height);
	CGContextSetFillColorWithColor(ctx, [UIColor clearColor].CGColor);
	CGContextClearRect(ctx, rect);
	CGContextDrawImage(ctx, rect, img.CGImage);

	CGContextRelease(ctx);
	CGColorSpaceRelease(color_space);
    [img release];
    [path release];

    return bitmap;
}

static FtkBitmap* ftk_image_iphone_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
    return_val_if_fail(ftk_image_iphone_decoder_match(thiz, filename) == RET_OK, NULL);

    return load_image(filename);
}

static void ftk_image_iphone_decoder_destroy(FtkImageDecoder* thiz)
{
    if(thiz != NULL)
    {
        FTK_ZFREE(thiz, sizeof(thiz));
    }
}

FtkImageDecoder* ftk_image_iphone_decoder_create(void)
{
    FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

    if(thiz != NULL)
    {
        thiz->match = ftk_image_iphone_decoder_match;
        thiz->decode = ftk_image_iphone_decoder_decode;
        thiz->destroy = ftk_image_iphone_decoder_destroy;
    }

    return thiz;
}
