#include "ftk.h"
#include "ftk_image_bmp_decoder.h"

int main(int argc, char* argv[])
{
	FtkBitmap* bitmap = NULL;
	FtkImageDecoder* decoder = NULL;
	
	ftk_init(argc, argv);

	decoder = ftk_image_bmp_decoder_create();
	bitmap = ftk_image_decoder_decode(decoder, argv[1]);
	
	if(bitmap != NULL)
	{
		ftk_bitmap_unref(bitmap);
	}
	ftk_image_decoder_destroy(decoder);

	return 0;
}
