#include "ftk.h"

void create_dialog(FtkBitmap* bitmap, FtkColor bg);
static void create_app_window(void);

static Ret button_open_image_dialog(void* ctx, void* obj)
{
	char filename[FTK_MAX_PATH+1] = {0};
	FtkBitmap* bitmap = NULL;
	FtkColor   bg = {0};
	bg.r = 0xff;
	bg.g = 0xff;
	bg.b = 0xff;
	ftk_snprintf(filename, FTK_MAX_PATH, "%s/earth.png", TESTDATA_DIR);
	bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
	bg.a = 0x0;
	create_dialog(bitmap, bg);

	return RET_OK;
}

static Ret button_open_transparent_dialog(void* ctx, void* obj)
{
	FtkColor   bg = {0};
	bg.a = 0x80;
	bg.r = 0xff;
	bg.g = 0xff;
	bg.b = 0xff;
	create_dialog(NULL, bg);

	return RET_OK;
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

static Ret button_quit_clicked(void* ctx, void* obj)
{
	*(int*)ctx = ftk_widget_id(obj);

	return RET_QUIT;
}

static Ret button_close_clicked(void* ctx, void* obj)
{
	ftk_widget_unref(ctx);

	return RET_OK;
}

void create_dialog(FtkBitmap* bitmap, FtkColor bg)
{
	int id = 0;
	int width = ftk_display_width(ftk_default_display());
	int height = ftk_display_height(ftk_default_display());
	FtkWidget* button = NULL;
	FtkWidget* label = NULL;
	FtkWidget* dialog = ftk_dialog_create(0, 5, 320, height-60);
	
	ftk_widget_set_attr(dialog, FTK_ATTR_BG_CENTER);
	ftk_window_set_background_with_alpha(dialog, bitmap, bg);

	width = ftk_widget_width(dialog);
	height = ftk_widget_height(dialog);
	
	//label = ftk_label_create(dialog, width/8, height/4, 3*width/4, 20);
	//ftk_widget_set_text(label, "Are you sure to quit?");

	label = ftk_label_create(dialog, width/8, height/2, 3*width/4, 20);
	ftk_widget_set_text(label, "Are you sure to quit?");

	button = ftk_button_create(dialog, width/6, 3*height/4, width/3, 50);
	ftk_widget_set_text(button, "yes");
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	
	button = ftk_button_create(dialog, width/2, 3*height/4, width/3, 50);
	ftk_widget_set_text(button, "no");
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	ftk_window_set_focus(dialog, button);

	ftk_widget_set_text(dialog, "transparent demo");
	ftk_widget_show_all(dialog, 1);

	assert(ftk_dialog_run(dialog) == id);
	ftk_widget_unref(dialog);

	return;
}

static void create_app_window(void)
{
	int width = 0;
	int height = 0;
	FtkWidget* win = ftk_app_window_create();
	FtkWidget* button = NULL;
	
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	button = ftk_button_create(win, 0, height/6, width/3, 50);
	ftk_widget_set_text(button, "图片背景");
	ftk_button_set_clicked_listener(button, button_open_image_dialog, win);

	button = ftk_button_create(win, 2*width/3, height/6, width/3, 50);
	ftk_widget_set_text(button, "半透明效果");
	ftk_button_set_clicked_listener(button, button_open_transparent_dialog, win);

	button = ftk_button_create(win, width/4, height/2, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_close_clicked, win);

	ftk_widget_set_text(win, "transparent");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	return;
}

int FTK_MAIN(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	create_app_window();

	ftk_run();

	return 0;
}

