#include "ftk.h"

static void create_app_window(void);
static Ret button_open_clicked(void* ctx, void* obj)
{
	create_app_window();

	return RET_OK;
}

static Ret button_close_clicked(void* ctx, void* obj)
{
	FtkWidget* win = ctx;
	ftk_logd("%s: close window %s\n", __func__, ftk_window_get_title(win));
	ftk_widget_unref(win);

	return RET_OK;
}

static Ret button_default_clicked(void* ctx, void* obj)
{
	printf("%s: button %s is clicked.\n", __func__, ftk_button_get_text(obj));

	return RET_OK;
}


static int g_index = 0;
static void on_window_close(void* user_data)
{
	g_index--;

	ftk_logd("%s: g_index=%d\n", __func__, g_index);
	if(g_index == 0)
	{
		ftk_quit();
	}

	return ;
}
static void create_app_window(void)
{
	char title[32] = {0};
	FtkWidget* win = ftk_window_create(0, 2, 320, 478);
	FtkWidget* label = NULL;
	FtkWidget* button = ftk_button_create(1001, 10, 30, 120, 30);
	ftk_button_set_text(button, "打开新窗口");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_open_clicked, win);

	button = ftk_button_create(1002, 180, 30, 120, 30);
	ftk_button_set_text(button, "关闭当前窗口");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_close_clicked, win);

	snprintf(title, sizeof(title), "window%02d", g_index++);
	label = ftk_label_create(1003, 100, 200, 120, 30);
	ftk_label_set_text(label, title);
	ftk_widget_show(label, 1);
	ftk_widget_append_child(win, label);
	
	ftk_window_set_title(win, title);
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	create_app_window();

	ftk_run();

	return 0;
}

