#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

static Ret timer(void* user_data)
{
	int x = 0;
	int y = 0;
	FtkSprite* sprite = user_data;
	FtkDisplay* display = ftk_default_display();

	x = ftk_sprite_get_x(sprite);
	y = ftk_sprite_get_y(sprite);

	x += 5;
	y += 5;
	ftk_sprite_move(sprite, x, y);

	return x < ftk_display_width(display) && y < ftk_display_height(display) ? RET_OK : RET_REMOVE;
}

Ret on_move(void* ctx, void* obj)
{
	ftk_logd("%s: %d %d\n", __func__, ftk_sprite_get_x(obj), ftk_sprite_get_y(obj));

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkSprite* sprite = NULL;
	FtkBitmap* icon = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	button = ftk_button_create(win, width/4, height/2, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "sprite demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	sprite = ftk_sprite_create();
	icon=ftk_icon_cache_load(ftk_default_icon_cache(), "flag-32"FTK_STOCK_IMG_SUFFIX);
	ftk_sprite_set_icon(sprite, icon);
	ftk_sprite_set_move_listener(sprite, on_move, NULL);
	ftk_sprite_show(sprite, 1);
	
	ftk_main_loop_add_source(ftk_default_main_loop(), ftk_source_timer_create(500, timer, sprite));
	ftk_run();

	return 0;
}

