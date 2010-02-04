#include "ftk_globals.h"
#include "ftk_bitmap.h"
#include "ftk_allocator_default.h"

int main(int argc, char* argv[])
{
	int i = 0;
	int w = 100;
	int h = 100;
	FtkColor bg = {0xff,0xff,0xff,0xff};
	ftk_set_allocator(ftk_allocator_default_create());
	FtkBitmap* thiz = ftk_bitmap_create(w, h, bg);

	assert(ftk_bitmap_width(thiz) == w);
	assert(ftk_bitmap_height(thiz) == h);
	for(i = 0; i <(w *h); i++)
	{
		assert(memcmp(ftk_bitmap_bits(thiz)+i, &bg, sizeof(FtkColor)) == 0);
	}

	ftk_bitmap_unref(thiz);

	return 0;
}

