#include "ftk.h"

static Ret timeout(void* ctx)
{
	ftk_quit();

	ftk_logd("%s: timeout and quit.\n", __func__);

	return RET_REMOVE;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	FtkSource* timer = ftk_source_timer_create(5000, timeout, NULL);
	FtkWidget* win = ftk_window_create(0, 0, 320, 480);

	FtkWidget* image = ftk_image_create(1001, 0, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, "testdata/png1.png");
	ftk_widget_append_child(win, image);
	ftk_widget_show(image, 1);
	
	image = ftk_image_create(1001, ftk_widget_width(win)/2, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, "testdata/png2.png");
	ftk_widget_append_child(win, image);
	ftk_widget_show(image, 1);
	
	image = ftk_image_create(1001, 0, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, "testdata/png3.png");
	ftk_widget_append_child(win, image);
	ftk_widget_show(image, 1);
	
	image = ftk_image_create(1001, ftk_widget_width(win)/2, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, "testdata/jpeg1.jpg");
	ftk_widget_append_child(win, image);
	ftk_widget_show(image, 1);

	ftk_window_set_title(win, "demo3");
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	ftk_widget_show(win, 1);

	ftk_run();

	return 0;
}
