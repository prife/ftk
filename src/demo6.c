#include "ftk.h"
#include "ftk_status_item.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
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

static void init_panel(void)
{
	FtkGc gc = {0};
	FtkWidget* item = NULL;	
	FtkWidget* panel = ftk_default_status_panel();

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
	ftk_status_item_set_clicked_listener(item, button_quit_clicked, NULL);

	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/flag-32.png");
	item = ftk_status_item_create(1, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(item, FTK_WIDGET_FOCUSED, &gc);
	ftk_status_panel_add(panel, 1, item);
	ftk_widget_show(item, 1);

	item = ftk_status_item_create(1, 160, 32);
	ftk_button_set_text(item, "MailSystem");
	ftk_status_panel_add(panel, 2, item);
	ftk_widget_show(item, 1);

	ftk_widget_show(panel, 1);

	return;
}

static void init_app_window(void)
{
	FtkWidget* win = ftk_window_create(0, 2, 320, 478);

	FtkWidget* button = ftk_button_create(1001, 10, 30, 80, 30);
	ftk_button_set_text(button, "show");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_show_clicked, win);

	button = ftk_button_create(1002, 100, 30, 80, 30);
	ftk_button_set_text(button, "hide");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_hide_clicked, win);
	
	button = ftk_button_create(1003, 200, 30, 80, 30);
	ftk_button_set_text(button, "按钮测试");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1004, 10, 130, 80, 40);
	ftk_button_set_text(button, "yes");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1005, 100, 130, 80, 40);
	ftk_button_set_text(button, "no");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, win);
	
	button = ftk_button_create(1006, 200, 130, 80, 40);
	ftk_button_set_text(button, "quit");
	ftk_widget_append_child(win, button);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_window_set_title(win, "button demo");
	ftk_widget_show(win, 1);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	init_panel();
	init_app_window();

	ftk_run();

	return 0;
}

