#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	*(int*)ctx = ftk_widget_id(obj);

	return RET_QUIT;
}

static Ret button_hide_clicked(void* ctx, void* obj)
{
	ftk_widget_show(ftk_widget_lookup(ctx, 1003), 0);

	return RET_OK;
}

static Ret button_show_clicked(void* ctx, void* obj)
{
	ftk_widget_show(ftk_widget_lookup(ctx, 1003), 1);

	return RET_OK;
}

static Ret button_default_clicked(void* ctx, void* obj)
{
	printf("%s: button %s is clicked.\n", __func__, ftk_button_get_text(obj));

	return RET_OK;
}

static void on_window_close(void* user_data)
{
	FtkEvent event = {0};
	ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);

	return ;
}

int main(int argc, char* argv[])
{
	int id = 0;
	ftk_init(argc, argv);
	
	FtkWidget* dialog = ftk_dialog_create(0, 30, 320, 300);
	FtkWidget* button = ftk_button_create(1001, 10, 30, 80, 30);
	ftk_button_set_text(button, "show");
	ftk_widget_append_child(dialog, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_show_clicked, dialog);

	button = ftk_button_create(1002, 100, 30, 80, 30);
	ftk_button_set_text(button, "hide");
	ftk_widget_append_child(dialog, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_hide_clicked, dialog);
	
	button = ftk_button_create(1003, 200, 30, 80, 30);
	ftk_button_set_text(button, "按钮测试");
	ftk_widget_append_child(dialog, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, dialog);
	
	button = ftk_button_create(1005, 20, 120, 80, 40);
	ftk_button_set_text(button, "yes");
	ftk_widget_append_child(dialog, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	
	button = ftk_button_create(1006, 180, 120, 80, 40);
	ftk_button_set_text(button, "no");
	ftk_widget_append_child(dialog, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	ftk_window_set_focus(dialog, button);

	ftk_window_set_title(dialog, "dialog demo");
	ftk_widget_show(dialog, 1);
	ftk_widget_set_user_data(dialog, on_window_close, dialog);

	assert(ftk_dialog_run(dialog) == id);

	return 0;
}

