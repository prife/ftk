#include "ftk.h"

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* progress_bar = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width/3 - 10;
	progress_bar = ftk_progress_bar_create(1001,0, 30, width, 50);
	ftk_progress_bar_set_percent(progress_bar, 20);
	ftk_widget_append_child(win, progress_bar);
	ftk_widget_show(progress_bar, 1);

	ftk_window_set_title(win, "progress_bar demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

