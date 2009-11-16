#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static Ret button_clicked(void* ctx, void* obj)
{
	return RET_OK;
}

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
	FtkWidget* button = NULL;
	FtkWidget* group = NULL;
	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width/2 - 10;
	button = ftk_check_button_create(1001,0, 30, width, 50);
	ftk_widget_set_text(button, "show");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_clicked, win);

	button = ftk_check_button_create(1002, width + 10, 30, width, 50);
	ftk_widget_set_text(button, "hide");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_clicked, win);
	
	group = ftk_radio_group_create(2000, 0, 90, 2 * width, 60);
	ftk_widget_show(group, 1);
	ftk_widget_append_child(win, group);
	
	button = ftk_radio_button_create(1001,0, 10, width, 50);
	ftk_widget_set_text(button, "Male");
	ftk_widget_append_child(group, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_clicked, win);

	button = ftk_radio_button_create(1002, width + 10, 10, width, 50);
	ftk_widget_set_text(button, "Female");
	ftk_widget_append_child(group, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_clicked, win);
	
	button = ftk_button_create(1006, width/2, height/2 + 10, width, 80);
	ftk_widget_set_text(button, "quit");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "button demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

