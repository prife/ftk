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
	ftk_logd("%s: close window %s\n", __func__, ftk_widget_get_text(win));
	ftk_widget_unref(win);

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

static Ret on_prepare_options_menu(void* ctx, FtkWidget* menu_panel)
{
	int i = 0;
	for(i = 0; i < g_index && i < 8; i++)
	{
		char text[32] = {0};
		FtkWidget* item = ftk_menu_item_create(1000);
		snprintf(text, sizeof(text), "Menu%02d", i);
		ftk_widget_set_text(item, text);
		ftk_widget_show(item, 1);
		ftk_menu_panel_add(menu_panel, item);
	}

	return i > 0 ? RET_OK : RET_FAIL;
}

static void create_app_window(void)
{
	int width = 0;
	int height = 0;
	char title[32] = {0};
	FtkWidget* win = ftk_app_window_create();
	FtkWidget* label = NULL;
	FtkWidget* button = NULL;

	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	button = ftk_button_create(1001, 0, height/6, width/3, 50);
	ftk_widget_set_text(button, "打开新窗口");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_open_clicked, win);

	button = ftk_button_create(1002, 2*width/3, height/6, width/3, 50);
	ftk_widget_set_text(button, "关闭当前窗口");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_close_clicked, win);

	label = ftk_label_create(1003, 10, ftk_widget_height(win)/2, ftk_widget_width(win)-20, 30);
	ftk_label_set_text(label, "press F2 to open menu");
	ftk_widget_show(label, 1);
	ftk_widget_append_child(win, label);
	
	snprintf(title, sizeof(title), "window%02d", g_index++);
	ftk_widget_set_text(win, title);
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);
	
	ftk_app_window_set_on_prepare_options_menu(win, on_prepare_options_menu, win);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	create_app_window();

	ftk_run();

	return 0;
}

