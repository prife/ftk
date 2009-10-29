#include "ftk.h"

typedef struct _TimerInfo
{
	int times;
	FtkWidget* label;
}TimerInfo;

static Ret timeout(void* ctx)
{
	TimerInfo* info = ctx;
	char buffer[32] = {0};
	if(info->times > 0)
	{
		snprintf(buffer, sizeof(buffer), "Quit after %d seconds", info->times);
		ftk_label_set_text(info->label, buffer);
		info->times--;

		return RET_OK;
	}
	else
	{
		ftk_quit();
		ftk_logd("%s: timeout and quit.\n", __func__);
	
		return RET_REMOVE;
	}
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

int main(int argc, char* argv[])
{
	TimerInfo info = {.times=5, };
	ftk_init(argc, argv);
	
	FtkSource* timer = ftk_source_timer_create(1000, timeout, &info);
	FtkWidget* win = ftk_window_create(0, 0, 320, 480);

	FtkWidget* label = ftk_label_create(1001, 10, 30, 160, 20);
	ftk_label_set_text(label, "中文文本");
	ftk_widget_append_child(win, label);
	ftk_widget_show(label, 1);
	
	label = ftk_label_create(1002, 10, 60, 160, 20);
	ftk_label_set_text(label, "English Text");
	ftk_widget_append_child(win, label);
	ftk_widget_show(label, 1);
	
	label = ftk_label_create(1003, 50, 200, 200, 20);
	ftk_widget_append_child(win, label);
	ftk_widget_show(label, 1);
	info.label = label;

	ftk_window_set_title(win, "demo1");

	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	ftk_widget_show(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}
