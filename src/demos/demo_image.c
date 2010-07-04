#include "ftk.h"

static Ret timeout(void* ctx)
{
	FTK_QUIT();

	ftk_logd("%s: timeout and quit.\n", __func__);

	return RET_REMOVE;
}

int FTK_MAIN(int argc, char* argv[])
{
	FtkSource* timer = NULL;
	FtkWidget* image = NULL;
	FtkWidget* win = NULL;
	char filename[FTK_MAX_PATH+1] = {0};
	FTK_INIT(argc, argv);
	
	timer = ftk_source_timer_create(5000, timeout, NULL);
	win = ftk_window_create(0, 0, 320, 480);

	ftk_snprintf(filename, FTK_MAX_PATH, "%s/earth.png", TESTDATA_DIR);
	image = ftk_image_create(win, 0, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image(image, 
		ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename));
	
	ftk_snprintf(filename, FTK_MAX_PATH, "%s/png_RGB_tRNS.png", TESTDATA_DIR);
	image = ftk_image_create(win, 0, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image(image, 
		ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename));
	ftk_widget_set_attr(image, FTK_ATTR_TRANSPARENT);

	ftk_snprintf(filename, FTK_MAX_PATH, "%s/Calculator.png", TESTDATA_DIR);
	image = ftk_image_create(win, ftk_widget_width(win)/2, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image(image, 
		ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename));
	ftk_widget_set_attr(image, FTK_ATTR_BG_TILE);

	ftk_snprintf(filename, FTK_MAX_PATH, "%s/t8.bmp", TESTDATA_DIR);
	image = ftk_image_create(win, 0, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image(image, 
		ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename));
	ftk_widget_set_attr(image, FTK_ATTR_BG_CENTER);
	
	ftk_snprintf(filename, FTK_MAX_PATH,  "%s/jpeg1.jpg", TESTDATA_DIR);
	image = ftk_image_create(win, ftk_widget_width(win)/2, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image(image, 
		ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename));
	ftk_widget_set_attr(image, FTK_ATTR_BG_TILE);

	ftk_widget_set_text(win, "image demo");
	ftk_widget_show_all(win, 1);
	
	FTK_QUIT_WHEN_WIDGET_CLOSE(win);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);

	FTK_RUN();

	return 0;
}
