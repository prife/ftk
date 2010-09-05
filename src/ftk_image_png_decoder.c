/*
 * File: ftk_image_png_decoder.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   png format image decoder.
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define PNG_SKIP_SETJMP_CHECK
#include <png.h>
#include "ftk_log.h"
#include "ftk_image_png_decoder.h"

static Ret ftk_image_png_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(filename != NULL, RET_FAIL);

	return (strstr(filename, ".png") != NULL) ? RET_OK : RET_FAIL;
}

static FtkBitmap* load_png (const char *filename)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int passes_nr = 0;
	FILE *fp = NULL;	
	FtkColor* dst = NULL;
	unsigned char* src = NULL;
	FtkBitmap* bitmap = NULL;
	FtkColor  bg = {0};
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep * row_pointers = NULL;

	bg.a = 0xff;
	if ((fp = fopen (filename, "rb")) == NULL)
	{
		ftk_logd("%s: open %s failed.\n", __func__, filename);
		return NULL;
	}

	if((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
	{
		fclose(fp);
		return NULL;
	}
	
	if((info_ptr = png_create_info_struct(png_ptr)) == NULL)
	{
		fclose(fp);
		return NULL;
	}

	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, fp);
	memset(info_ptr, 0x00, sizeof(*info_ptr));
	png_read_info(png_ptr, info_ptr);

	w = info_ptr->width;
	h = info_ptr->height;

	passes_nr = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	setjmp(png_jmpbuf(png_ptr));

	row_pointers = (png_bytep*) FTK_ZALLOC(h * sizeof(png_bytep));
	for (y=0; y< h; y++)
	{
		row_pointers[y] = (png_byte*) FTK_ZALLOC(info_ptr->rowbytes);
	}
	png_read_image(png_ptr, row_pointers);

	bitmap = ftk_bitmap_create(w, h, bg);
	dst = ftk_bitmap_bits(bitmap);
	if (info_ptr->color_type == PNG_COLOR_TYPE_RGBA)
	{
		for(y = 0; y < h; y++)
		{
			src = row_pointers[y];
			for(x = 0; x < w; x++)
			{
				if(src[3])
				{
					dst->r = src[0];
					dst->g = src[1];
					dst->b = src[2];
				}
				dst->a = src[3];
				src +=4;
				dst++;
			}
		}
	}
	else if(info_ptr->color_type == PNG_COLOR_TYPE_RGB)
	{
		if(0 == info_ptr->num_trans)
		{
			for(y = 0; y < h; y++)
			{
				src = row_pointers[y];
				for(x = 0; x < w; x++)
				{
					dst->r = src[0];
					dst->g = src[1];
					dst->b = src[2];
					dst->a = 0xff;
					src += 3;
					dst++;
				}
			}
		}
		else 
		{
			/* According to PNG_EXPAND_tRNS to build in an alpha channel using trans_value. */
			png_byte red = png_ptr->trans_values.red & 0xff;
			png_byte green = png_ptr->trans_values.green & 0xff;
			png_byte blue = png_ptr->trans_values.blue & 0xff;
			for(y = 0; y < h; y++)
			{
				src = row_pointers[y];
				for(x = 0; x < w; x++)
				{
					if(src[0] == red && src[1] == green && src[2] == blue)
					{
						dst->a = 0;
					}
					else
					{
						dst->a = 0xff;
					}
					dst->r = src[0];
					dst->g = src[1];
					dst->b = src[2];
					src += 3;
					dst++;
				}
			}
		}
	}
	else
	{
		assert(!"not supported.");
	}

	for(y = 0; y < h; y++)
	{
		FTK_FREE(row_pointers[y]);
	}
	FTK_FREE(row_pointers);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL); 
	fclose(fp);

	return bitmap;
}

static FtkBitmap* ftk_image_png_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(ftk_image_png_decoder_match(thiz, filename) == RET_OK, NULL);

	return load_png(filename);
}

static void ftk_image_png_decoder_destroy(FtkImageDecoder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(thiz));
	}

	return;
}

FtkImageDecoder* ftk_image_png_decoder_create(void)
{
	FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

	if(thiz != NULL)
	{
		thiz->match   = ftk_image_png_decoder_match;
		thiz->decode  = ftk_image_png_decoder_decode;
		thiz->destroy = ftk_image_png_decoder_destroy;
	}

	return thiz;
}

