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
	int width = 0;
	int height = 0;
	FtkGc gc = {.mask = FTK_GC_BG};
	TimerInfo info = {.times=5, };
	ftk_init(argc, argv);
		
	FtkSource* timer = ftk_source_timer_create(1000, timeout, &info);
	FtkWidget* win = ftk_app_window_create();

	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	FtkWidget* label = ftk_label_create(win, 10, 10, width - 20, 20);
	ftk_label_set_text(label, "中文文本");
	
	label = ftk_label_create(win, 10, 40, width - 20, 20);
	ftk_label_set_text(label, "English Text");
	assert(strcmp(ftk_label_get_text(label), "English Text") == 0);
	
	gc.bg.a = 0xff;
	gc.bg.r = 0xF0;
	gc.bg.g = 0xF0;
	gc.bg.b = 0x80;
	label = ftk_label_create(win, 10, height/2, width - 20, 120);
	ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
	ftk_widget_unset_attr(label, FTK_ATTR_TRANSPARENT);
	ftk_label_set_text(label, "中英文混合多行文本显示:the linux mobile development.带有背景颜色");
	
	label = ftk_label_create(win, 50, height/2-30, width, 20);
	info.label = label;
	
	ftk_widget_set_text(win, "label demo");
	ftk_widget_show_all(win, 1);

	ftk_widget_set_user_data(win, on_window_close, win);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);

	ftk_run();

	return 0;
}

