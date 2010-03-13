/*
 * File: ftk_image_jpeg_decoder.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   
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
#include "ftk_image_jpeg_decoder.h"
#include <setjmp.h>
#include <jpeglib.h>
#include "ftk_mmap.h"

static Ret ftk_image_jpeg_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
	unsigned char* data = NULL;
	size_t length = 0;
	char identifier[4] = {0};
	FtkMmap* m = ftk_mmap_create(filename, 0, -1);

	if(m == NULL) return RET_FAIL;

	data = ftk_mmap_data(m);
	length = ftk_mmap_length(m);

	/* check SOI */
	if (data[0] != 0xFF || data[1] != 0xD8)
		goto fail;	/* not JPEG image */
	/* check Identifier */
	memcpy(identifier, data+6, sizeof(identifier));
	if (strncmp(identifier, "JFIF", 4) != 0 && strncmp(identifier, "Exif", 4) != 0)
		goto fail;
	/* check EOI */
	if (data[length-2] == 0xFF && data[length-1] == 0xD9) 
	{
		ftk_mmap_destroy(m);
		return RET_OK;
	} 
	else
	{
		fprintf(stderr, "%s: JPEG EOI is incorrect: 0x%02X%02X\n", __func__, data[length-2], data[length-1]);
		goto fail;	/* JPEG image has no end */
	}
fail:
	ftk_mmap_destroy(m);
	return RET_FAIL;
}

struct my_error_mgr
{
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr *my_error_ptr;
static void put_scanline_to_bitmap (FtkBitmap* bitmap, char *scanline, int y, int width)
{
	int i = 0;
	FtkColor* dst = NULL;
	char *pixels = scanline;

	dst = ftk_bitmap_bits(bitmap) + y * width;

	while (i < width)
	{
		dst->r = pixels[0];
		dst->g = pixels[1];
		dst->b = pixels[2];

		i++;
		dst++;
		pixels += 3;
	}

	return;
}

METHODDEF (void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	(*cinfo->err->output_message) (cinfo);

	longjmp (myerr->setjmp_buffer, 1);
}
static FtkBitmap* load_jpeg (const char *filename)
{
	int i = 0;
	FILE *infile;	
	JSAMPARRAY buffer;	
	int row_stride;	
	FtkColor bg = {0};
	FtkBitmap* bitmap = NULL;
	struct my_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;
	
	bg.a = 0xff;
	if ((infile = fopen (filename, "rb")) == NULL)
	{
		fprintf (stderr, "can't open %s\n", filename);
		return 0;
	}

	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress (&cinfo);
		fclose (infile);
		return NULL;
	}
	jpeg_create_decompress (&cinfo);
	jpeg_stdio_src (&cinfo, infile);

	(void) jpeg_read_header (&cinfo, TRUE);

	(void) jpeg_start_decompress (&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);
	
	if((bitmap = ftk_bitmap_create(cinfo.output_width, cinfo.output_height, bg)) == NULL)
	{
		jpeg_destroy_decompress (&cinfo);
		fclose (infile);

		return NULL;
	}

	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void) jpeg_read_scanlines (&cinfo, buffer, 1);

		put_scanline_to_bitmap (bitmap, (char*)buffer[0], i, cinfo.output_width);
		i++;
	}

	(void) jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);

	fclose (infile);

	return bitmap;
}
static FtkBitmap* ftk_image_jpeg_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(ftk_image_jpeg_decoder_match(thiz, filename) == RET_OK, NULL);

	return load_jpeg(filename);
}

static void ftk_image_jpeg_decoder_destroy(FtkImageDecoder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(thiz));
	}

	return;
}

FtkImageDecoder* ftk_image_jpeg_decoder_create(void)
{
	FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

	if(thiz != NULL)
	{
		thiz->match   = ftk_image_jpeg_decoder_match;
		thiz->decode  = ftk_image_jpeg_decoder_decode;
		thiz->destroy = ftk_image_jpeg_decoder_destroy;
	}

	return thiz;
}

#ifdef FTK_IMAGE_DECODER_JPEG_TEST
int main(int argc, char* argv[])
{
	FtkBitmap* bitmap = NULL;
	const char* filename = argv[1];
	FtkImageDecoder* thiz = ftk_image_jpeg_decoder_create();
	
	assert(ftk_image_decoder_match(thiz, filename) == RET_OK);
	assert((bitmap = ftk_image_decoder_decode(thiz, filename)) != NULL);
	printf("w=%d h=%d\n", ftk_bitmap_width(bitmap), ftk_bitmap_height(bitmap));
	ftk_bitmap_unref(bitmap);
	ftk_image_decoder_destroy(thiz);

	return 0;
}
#endif/*FTK_IMAGE_JPEG_TEST*/
