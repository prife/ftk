#include "ftk.h"
#include "ftk_status_item.h"

typedef struct _AppInfo
{
	FtkWidget* panel;
	FtkWidget* title_widget;
	FtkWidget* top_window;
}AppInfo;

AppInfo g_appinfo = {0};

static void create_app_window(void);

static Ret on_wnd_manager_event(void* ctx, void* obj)
{
	AppInfo* info = ctx;
	FtkEvent* event = obj;
	Ret ret = RET_OK;
	switch(event->type)
	{
		case FTK_EVT_TOP_WND_CHANGED:
		{
			info->top_window = event->widget;
			ftk_logd("top_window changed\n");

			if(info->top_window != NULL)
			{
				ftk_status_item_set_text(info->title_widget, ftk_window_get_title(info->top_window));
			//	ftk_widget_paint(info->panel);
			}
			ret = RET_REMOVE;
			break;
		}
		case FTK_EVT_WND_CONFIG_CHANGED:
		{
			if(info->top_window == event->widget)
			{
				ftk_status_item_set_text(info->title_widget, ftk_window_get_title(info->top_window));
			}
			//ftk_widget_paint(info->panel);
			ftk_logd("%s: config changed: %p %p\n", __func__, info->top_window, event->widget);
			ret = RET_REMOVE;
			break;
		}
		default:break;
	}

	return ret;
}

static Ret button_close_top_clicked(void* ctx, void* obj)
{
	AppInfo* info = ctx;

	if(info->top_window != NULL)
	{
		ftk_logd("%s: close window %s\n", __func__, ftk_window_get_title(info->top_window));
		ftk_widget_unref(info->top_window);
		info->top_window = NULL;
	}

	return RET_OK;
}

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

static void init_panel(void)
{
	FtkGc gc = {0};
	FtkWidget* item = NULL;	
	FtkWidget* panel = ftk_default_status_panel();

	g_appinfo.panel = panel;
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/status-bg.png");
	ftk_widget_set_gc(panel, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(panel, FTK_WIDGET_FOCUSED, &gc);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/close-32.png");
	item = ftk_status_item_create(1, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
	ftk_status_panel_add(panel, -1, item);
	ftk_widget_show(item, 1);
	ftk_status_item_set_clicked_listener(item, button_close_top_clicked, &g_appinfo);

	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/flag-32.png");
	item = ftk_status_item_create(1, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
	ftk_status_panel_add(panel, 1, item);
	ftk_widget_show(item, 1);
	ftk_status_item_set_clicked_listener(item, button_default_clicked, NULL);

	item = ftk_status_item_create(1, 160, 32);
	ftk_button_set_text(item, "MailSystem");
	ftk_status_panel_add(panel, 2, item);
	ftk_widget_show(item, 1);
	ftk_status_item_set_clicked_listener(item, button_default_clicked, NULL);
	g_appinfo.title_widget = item;

	ftk_wnd_manager_add_global_listener(ftk_default_wnd_manager(), on_wnd_manager_event, &g_appinfo);
	ftk_widget_show(panel, 1);

	return;
}

static int g_index = 0;
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

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	init_panel();
	create_app_window();

	ftk_run();

	return 0;
}

