#include "ftk.h"

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static Ret button_start_clicked(void* ctx, void* obj)
{
	ftk_wait_box_start_waiting(ctx);

	return RET_OK;
}

static Ret button_stop_clicked(void* ctx, void* obj)
{
	ftk_wait_box_stop_waiting(ctx);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* wait_box = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	wait_box = ftk_wait_box_create(1001, width/2 - 16, height/4);
	ftk_widget_append_child(win, wait_box);
	ftk_widget_show(wait_box, 1);
	
	button = ftk_button_create(1006, 0, height/2, width/3, 50);
	ftk_widget_set_text(button, "start");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_start_clicked, wait_box);
	
	button = ftk_button_create(1006, 2*width/3, height/2, width/3, 50);
	ftk_widget_set_text(button, "stop");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_stop_clicked, wait_box);

	button = ftk_button_create(1006, width/4, 3*height/4, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "wait_box demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

