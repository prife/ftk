#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static Ret button_hide_clicked(void* ctx, void* obj)
{
	ftk_widget_show(ftk_widget_lookup(ctx, 1003), 0);
	ftk_check_button_set_checked(obj, !ftk_check_button_get_checked(obj));

	return RET_OK;
}

static Ret button_show_clicked(void* ctx, void* obj)
{
	ftk_widget_show(ftk_widget_lookup(ctx, 1003), 1);
	ftk_check_button_set_checked(obj, !ftk_check_button_get_checked(obj));

	return RET_OK;
}

static Ret button_default_clicked(void* ctx, void* obj)
{
	printf("%s: button %s is clicked.\n", __func__, ftk_widget_get_text(obj));

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

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width/3 - 10;
	button = ftk_check_button_create(1001,0, 30, width, 50);
	ftk_widget_set_text(button, "show");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_show_clicked, win);

	button = ftk_check_button_create(1002, width + 10, 30, width, 50);
	ftk_widget_set_text(button, "hide");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_check_button_set_clicked_listener(button, button_hide_clicked, win);
	
	button = ftk_button_create(1003, 2*(width + 10), 30, width, 50);
	ftk_widget_set_text(button, "按钮测试");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1004, 0, 130, width, 40);
	ftk_widget_set_text(button, "yes");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1005, 2*(width + 10), 130, width, 40);
	ftk_widget_set_text(button, "no");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1006, width + 10, height/2, width, 80);
	ftk_widget_set_text(button, "quit");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_window_set_title(win, "button demo");
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

