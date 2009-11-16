#include "ftk.h"

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

static Ret update_progress(void* ctx)
{
	int percent = 0;
	FtkWidget* progress_bar = ctx;

	percent = ftk_progress_bar_get_percent(progress_bar);
	if(percent == 100)
	{
		return RET_REMOVE;
	}
	ftk_progress_bar_set_percent(progress_bar, percent + 10);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* progress_bar = NULL;
	FtkSource* timer = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width - 20;
	progress_bar = ftk_progress_bar_create(1001, 10, height/6, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	ftk_widget_append_child(win, progress_bar);
	ftk_widget_show(progress_bar, 1);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	
	progress_bar = ftk_progress_bar_create(1001, 10, height/3, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	ftk_widget_append_child(win, progress_bar);
	ftk_widget_show(progress_bar, 1);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	
	progress_bar = ftk_progress_bar_create(1001, 10, height/2, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	ftk_widget_append_child(win, progress_bar);
	ftk_widget_show(progress_bar, 1);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);

	ftk_widget_set_text(win, "progress_bar demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

