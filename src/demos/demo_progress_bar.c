#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
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

int FTK_MAIN(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* progress_bar = NULL;
	FtkSource* timer = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width - 20;
	progress_bar = ftk_progress_bar_create(win, 10, height/6, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	
	progress_bar = ftk_progress_bar_create(win, 10, height/3, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	
	progress_bar = ftk_progress_bar_create(win, 10, height/2, width, 20);
	ftk_progress_bar_set_percent(progress_bar, 20);
	timer = ftk_source_timer_create(1000, update_progress, progress_bar);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);

	button = ftk_button_create(win, width/4, 3*height/4, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "progress_bar demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_attr(win, FTK_ATTR_QUIT_WHEN_CLOSE);

	ftk_run();

	return 0;
}

