/*
 * File: ftk_image_win32_decoder.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   png/jpeg format image decoder on win32.
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
 * 2010-01-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_image_win32_decoder.h"
#define PNG_SKIP_SETJMP_CHECK
#include <win32.h>

static Ret ftk_image_win32_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(filename != NULL, RET_FAIL);

	return (strstr(filename, ".png") != NULL 
		|| strstr(filename, ".jpg") != NULL
		|| strstr(filename, ".jpeg") != NULL) ? RET_OK : RET_FAIL;
}

static FtkBitmap* load_win32 (const char *filename)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int n = 0;
	FILE *fp = NULL;	
	FtkColor* dst = NULL;
	unsigned char* src = NULL;
	FtkBitmap* bitmap = NULL;
	FtkColor  bg = {0};
	bg.a = 0xff;
	if ((fp = fopen (filename, "rb")) == NULL)
	{
		printf("%s: open %s failed.\n", __func__, filename);
		return NULL;
	}


	return bitmap;
}

static FtkBitmap* ftk_image_win32_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(ftk_image_win32_decoder_match(thiz, filename) == RET_OK, NULL);

	return load_win32(filename);
}

static void ftk_image_win32_decoder_destroy(FtkImageDecoder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(thiz));
	}

	return;
}

FtkImageDecoder* ftk_image_win32_decoder_create(void)
{
	FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

	if(thiz != NULL)
	{
		thiz->match   = ftk_image_win32_decoder_match;
		thiz->decode  = ftk_image_win32_decoder_decode;
		thiz->destroy = ftk_image_win32_decoder_destroy;
	}

	return thiz;
}

