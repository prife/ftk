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

static void init_panel(void)
{
	FtkGc gc = {0};
	FtkWidget* item = NULL;	
	FtkWidget* panel = ftk_default_status_panel();

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "testdata/maps-32.png");
	item = ftk_status_item_create(1, 32, 32);
	ftk_widget_set_gc(item, FTK_WIDGET_NORMAL, &gc);
	ftk_status_panel_add(panel, -1, item);
	ftk_widget_show(item, 1);
	
	item = ftk_status_item_create(1, 64, 32);
	ftk_button_set_text(item, "Demo");
	ftk_status_panel_add(panel, 1, item);
	ftk_widget_show(item, 1);

	ftk_widget_show(panel, 1);

	return;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	FtkWidget* win = ftk_window_create(0, 2, 320, 478);
	init_panel();
	ftk_widget_show(win, 1);

	ftk_run();

	return 0;
}

