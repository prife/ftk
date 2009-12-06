#include "ftk.h"

static Ret timeout(void* ctx)
{
	ftk_quit();

	ftk_logd("%s: timeout and quit.\n", __func__);

	return RET_REMOVE;
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	FtkSource* timer = ftk_source_timer_create(5000, timeout, NULL);
	FtkWidget* win = ftk_window_create(0, 0, 320, 480);

	FtkWidget* image = ftk_image_create(win, 0, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, TESTDATA_DIR"/earth.png");
	
	image = ftk_image_create(win, ftk_widget_width(win)/2, 0, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, LOCAL_DATA_DIR"/theme/default/icon_view_bg.png");
	ftk_widget_set_attr(image, FTK_ATTR_BG_TILE);
	
	image = ftk_image_create(win, 0, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, TESTDATA_DIR"/Calculator.png");
	ftk_widget_set_attr(image, FTK_ATTR_BG_CENTER);
	
	image = ftk_image_create(win, ftk_widget_width(win)/2, ftk_widget_height(win)/2, ftk_widget_width(win)/2, ftk_widget_height(win)/2);
	ftk_image_set_image_file(image, TESTDATA_DIR"/jpeg1.jpg");
	ftk_widget_set_attr(image, FTK_ATTR_BG_TILE);

	ftk_widget_set_text(win, "image demo");
	ftk_widget_show_all(win, 1);
	
	ftk_widget_set_user_data(win, on_window_close, win);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);

	ftk_run();

	return 0;
}
