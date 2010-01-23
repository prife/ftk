#include "ftk.h"

static const char* buttons[] = {"Yes", "No"};
static Ret button_warning(void* ctx, void* obj)
{
	int ret = ftk_warning("Warning", "It is danger!", buttons);

	ftk_logd("%s: ret = %d\n", __func__, ret);

	return RET_OK;
}

static Ret button_info(void* ctx, void* obj)
{
	int ret = ftk_infomation("Infomation", "Network is connected!", buttons);
	return RET_OK;
}

static Ret button_question(void* ctx, void* obj)
{
	int ret = ftk_question("Question", "Are you sure to quit?", buttons);

	return RET_OK;
}

static Ret button_quit(void* ctx, void* obj)
{
	ftk_quit();

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

	button = ftk_button_create(win, width/4, 0, width/2, 50);
	ftk_widget_set_text(button, "warning");
	ftk_button_set_clicked_listener(button, button_warning, win);
	
	button = ftk_button_create(win, width/4, 60, width/2, 50);
	ftk_widget_set_text(button, "question");
	ftk_button_set_clicked_listener(button, button_question, win);
	
	button = ftk_button_create(win, width/4, 120, width/2, 50);
	ftk_widget_set_text(button, "info");
	ftk_button_set_clicked_listener(button, button_info, win);
	
	button = ftk_button_create(win, width/4, 180, width/2, 50);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit, win);

	ftk_widget_set_text(win, "message box demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

