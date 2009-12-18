#include "ftk_cairo.h"

cairo_surface_t* ftk_cairo_surface_create(FtkWidget* window)
{
	int width = 0;
	int height = 0;
	FtkBitmap* bitmap = NULL;
	FtkCanvas* canvas = NULL;
	FtkColor*  data   = NULL;
	cairo_surface_t*  surface = NULL;
	return_val_if_fail(window != NULL, NULL);
	canvas = ftk_widget_canvas(window);
	bitmap = ftk_canvas_bitmap(canvas);
	return_val_if_fail(bitmap != NULL, NULL);

	data = ftk_bitmap_bits(bitmap);
	width = ftk_bitmap_width(bitmap);
	height = ftk_bitmap_height(bitmap);

	surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, width, height, width * 4);

	return surface;
}

