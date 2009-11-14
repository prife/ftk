#include "ftk.h"
#include "ftk_animator_expand.h"

static void create_ani_window(int type, int sync);
static void create_app_window(void);

static Ret button_to_east_south_clicked(void* ctx, void* obj)
{
	create_ani_window(FTK_ANI_TO_EAST_SOUTH, 1);

	return RET_OK;
}

static Ret button_to_right_clicked(void* ctx, void* obj)
{
	create_ani_window(FTK_ANI_TO_RIGHT, 0);

	return RET_OK;
}

static Ret button_to_down_clicked(void* ctx, void* obj)
{
	create_ani_window(FTK_ANI_TO_DOWN, 0);

	return RET_OK;
}

static Ret button_close_clicked(void* ctx, void* obj)
{
	FtkWidget* win = ctx;
	ftk_logd("%s: close window %s\n", __func__, ftk_window_get_title(win));
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

static void create_ani_window(int type, int sync)
{
	char title[32] = {0};
	int width = 0;
	int height = 0;
	FtkGc gc = {.mask = FTK_GC_BITMAP};
	FtkWidget* win = ftk_app_window_create();
	FtkAnimator* ani = ftk_animator_expand_create();
	
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "testdata/jpeg1.jpg");
	ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
	switch(type)
	{
		case FTK_ANI_TO_RIGHT:
		case FTK_ANI_TO_EAST_SOUTH:
		{
			ftk_animator_set_param(ani, type, 100, width, 50, 200);
			break;
		}
		case FTK_ANI_TO_DOWN:
		{
			ftk_animator_set_param(ani, type, 100, height, 50, 200);
			break;
		}
		default:break;
	}
	ftk_animator_start(ani, win, sync);

	return;
}

static void create_app_window(void)
{
	char title[32] = {0};
	int width = 0;
	int height = 0;
	FtkWidget* win = ftk_app_window_create();
	FtkWidget* label = NULL;
	FtkWidget* button = NULL;
	
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	button = ftk_button_create(1001, 0, height/6, width/3, 50);
	ftk_button_set_text(button, "向右伸展");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_to_right_clicked, win);

	button = ftk_button_create(1002, 2*width/3, height/6, width/3, 50);
	ftk_button_set_text(button, "向下伸展");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_to_down_clicked, win);

	button = ftk_button_create(1001, 0, height/6 + 80, width/3, 50);
	ftk_button_set_text(button, "向右下伸展");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_to_east_south_clicked, win);

	snprintf(title, sizeof(title), "window%02d", g_index++);
	label = ftk_label_create(1003, width/4, height/2, width/2, 30);
	ftk_label_set_text(label, title);
	ftk_widget_show(label, 1);
	ftk_widget_append_child(win, label);
	
	ftk_window_set_title(win, title);
	ftk_widget_set_user_data(win, on_window_close, win);
	
	ftk_widget_show(win, 1);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	create_app_window();

	ftk_run();

	return 0;
}

