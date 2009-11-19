#include "ftk.h"

int main(int argc, char* argv[])
{
	FtkBitmap* bitmap = NULL;
	ftk_init(argc, argv);

	FtkIconCache* thiz = ftk_default_icon_cache();
	bitmap = ftk_icon_cache_load(thiz, "close-32.png");
	assert(bitmap != NULL);
	ftk_bitmap_unref(bitmap);

	ftk_icon_cache_destroy(thiz);

	return 0;
}
