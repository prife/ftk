/*
 * File: ftk_image_bmp_decoder.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   bmp format image decoder.
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_mmap.h"
#include "ftk_image_bmp_decoder.h"
/**
 * http://atlc.sourceforge.net/bmp.html
 */
static Ret ftk_image_bmp_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(filename != NULL, RET_FAIL);

	return (strstr(filename, ".bmp") != NULL) ? RET_OK : RET_FAIL;
}

static Ret ftk_image_bmp_copy24(FtkBitmap* bitmap, int compress, char* data)
{
	return RET_OK;
}

static Ret ftk_image_bmp_copy8(FtkBitmap* bitmap, int compress, FtkColor* palette, char* data)
{
	return RET_OK;
}

static Ret ftk_image_bmp_copy4(FtkBitmap* bitmap, int compress, FtkColor* palette, char* data)
{
	return RET_OK;
}

static Ret ftk_image_bmp_copy1(FtkBitmap* bitmap, int compress, FtkColor* palette, char* data)
{
	return RET_OK;
}

static FtkBitmap* load_bmp (const char *filename)
{
	char* data = NULL;
	char* src = NULL;
	size_t length = 0;
	int compress=0;
	size_t bpp = 0;
	size_t width = 0;
	size_t height = 0;
	size_t colors = 0;
	size_t doffset = 0;
	FtkColor bg = {0};
	FtkBitmap* bitmap = NULL;
	FtkColor* palette = NULL;
	FtkMmap* m = ftk_mmap_create(filename, 0, -1);
	return_val_if_fail(m != NULL, NULL);

	data = ftk_mmap_data(m);
	length = ftk_mmap_length(m);

	if(data[0] != 'B' || data[1] != 'M')
	{
		ftk_mmap_destroy(m);

		return NULL;
	}

	bg.a = 0xff;
	doffset  = *(unsigned int*)(data + 0x000a);
	width    = *(unsigned int*)(data + 0x0012);
	height   = *(unsigned int*)(data + 0x0016);
	bpp      = *(unsigned short*)(data + 0x001c);
	compress = *(unsigned int*)(data + 0x001e);
	colors   = *(unsigned int*)(data + 0x002e);
	palette  = (FtkColor*)(data + 0x0036);
	src      = data + doffset;
	bitmap = ftk_bitmap_create(width, height, bg);

	switch(bpp)
	{
		case 24:
		{
			ftk_image_bmp_copy24(bitmap, compress, src);
			break;
		}
		case 8:
		{
			ftk_image_bmp_copy8(bitmap, compress, palette, src);
			break;
		}
		case 4:
		{
			ftk_image_bmp_copy4(bitmap, compress, palette, src);
			break;
		}
		case 1:
		{
			ftk_image_bmp_copy1(bitmap, compress, palette, src);
			break;
		}
		default:break;
	}

	ftk_mmap_destroy(m);

	return bitmap;
}

static FtkBitmap* ftk_image_bmp_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(ftk_image_bmp_decoder_match(thiz, filename) == RET_OK, NULL);

	return load_bmp(filename);
}

static void ftk_image_bmp_decoder_destroy(FtkImageDecoder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(thiz));
	}

	return;
}

FtkImageDecoder* ftk_image_bmp_decoder_create(void)
{
	FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

	if(thiz != NULL)
	{
		thiz->match   = ftk_image_bmp_decoder_match;
		thiz->decode  = ftk_image_bmp_decoder_decode;
		thiz->destroy = ftk_image_bmp_decoder_destroy;
	}

	return thiz;
}


