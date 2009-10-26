#include "ftk.h"

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

int main(int argc, char* argv[])
{
	FtkGc gc = {0};
	ftk_init(argc, argv);
	
	FtkWidget* win = ftk_window_create(0, 2, 320, 478);
	FtkWidget* close = ftk_status_item_create(1, 32, 32);
	
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "icons/home_32.png");
	ftk_widget_set_gc(close, FTK_WIDGET_NORMAL, &gc);
	ftk_status_panel_add(ftk_default_status_panel(), -1, close);
	ftk_widget_show(ftk_default_status_panel(), 1);
	ftk_widget_show(close, 1);
	ftk_widget_show(win, 1);

	ftk_run();

	return 0;
}

