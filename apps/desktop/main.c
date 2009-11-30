#include "ftk.h"
#include "ftk_animator_expand.h"

static FtkBitmap* load_bitmap(const char* filename)
{
	char path[260] = {0};
	FtkBitmap* bitmap = NULL;

	snprintf(path, sizeof(path), "%s/icons/%s", DESKTOP_DATA_DIR, filename);
	bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), path);
	if(bitmap == NULL)
	{
		snprintf(path, sizeof(path), "%s/icons/%s", DESKTOP_LOCAL_DATA_DIR, filename);
		bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), path);
	}

	return bitmap;
}

static Ret button_close_applist_clicked(void* ctx, void* obj)
{
	ftk_widget_unref(ctx);

	return RET_OK;
}

static Ret button_open_applist_clicked(void* ctx, void* obj)
{
	int delta = 0;
	int width  = 0;
	int height = 0;
	int button_height = 0;
	FtkGc gc = {0};
	FtkWidget* button = NULL;
	FtkWidget* win = ftk_app_window_create();
	FtkAnimator* ani = ftk_animator_expand_create();

	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_normal.png");
	button_height = ftk_bitmap_height(gc.bitmap);

	button = ftk_button_create(win, 0, 0, width, button_height);
	ftk_button_set_clicked_listener(button, button_close_applist_clicked, win);
	ftk_widget_set_attr(button, FTK_ATTR_BG_CENTER);
	ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
	ftk_gc_reset(&gc);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_selected.png");
	ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_pressed.png");
	ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
	ftk_gc_reset(&gc);

	delta = height/8;
	ftk_animator_set_param(ani, FTK_ANI_TO_UP, height - 100, ftk_widget_top(win), delta, 200);
	ftk_animator_start(ani, win, 0);

	ftk_widget_show_all(win, 1);

	return RET_OK;
}

Ret ftk_main(int argc, char* argv[])
{
	int width  = 0;
	int height = 0;
	int button_height = 0;
	FtkGc gc = {0};
	FtkWidget* button = NULL;
	FtkWidget* win = ftk_app_window_create();

	ftk_widget_set_attr(win, FTK_ATTR_IGNORE_CLOSE);
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_normal.png");
	button_height = ftk_bitmap_height(gc.bitmap);

	button = ftk_button_create(win, 0, height-button_height, width, button_height);
	ftk_button_set_clicked_listener(button, button_open_applist_clicked, win);
	ftk_widget_set_attr(button, FTK_ATTR_BG_CENTER);
	ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
	ftk_gc_reset(&gc);

	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_selected.png");
	ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = load_bitmap("tray_handle_pressed.png");
	ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
	ftk_gc_reset(&gc);

	ftk_widget_show_all(win, 1);

	return RET_OK;
}
