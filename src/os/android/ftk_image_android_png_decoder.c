
#include "ftk_image_android_png_decoder.h"

extern FtkBitmap* Android_LoadPng(const char *filename);

static Ret ftk_image_android_png_decoder_match(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(filename != NULL, RET_FAIL);

	return (strstr(filename, ".png") != NULL) ? RET_OK : RET_FAIL;
}

static FtkBitmap* ftk_image_android_png_decoder_decode(FtkImageDecoder* thiz, const char* filename)
{
	return_val_if_fail(ftk_image_android_png_decoder_match(thiz, filename) == RET_OK, NULL);

	return Android_LoadPng(filename);
}

static void ftk_image_android_png_decoder_destroy(FtkImageDecoder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(thiz));
	}
}

FtkImageDecoder* ftk_image_android_png_decoder_create(void)
{
	FtkImageDecoder* thiz = (FtkImageDecoder*)FTK_ZALLOC(sizeof(FtkImageDecoder));

	if(thiz != NULL)
	{
		thiz->match   = ftk_image_android_png_decoder_match;
		thiz->decode  = ftk_image_android_png_decoder_decode;
		thiz->destroy = ftk_image_android_png_decoder_destroy;
	}

	return thiz;
}
