#include "ftk.h"

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

#define TEXT_STR "Multi line editor:\nfirst\nsecond\nthird\nforth\n012345667890qwertyuiopasdfghjklkzxcbnmcutukakdfasiwerksjdfaksdjfaksdjifiwjkaldfkjkalsdfjieirnlkkafjiierklaaa\n1\n2\n3\n\n4\n5\n6\n7\n8\nlast line"


int FTK_MAIN(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* text_view  = NULL;

	ftk_init(argc, argv);

	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	text_view = ftk_text_view_create(win, 10, 10, ftk_widget_width(win) - 20, height/3);
	ftk_text_view_set_text(text_view, TEXT_STR);
	
	text_view = ftk_text_view_create(win, 10, 15 + height/3, ftk_widget_width(win) - 20, height/3);
	ftk_text_view_set_text(text_view, TEXT_STR);
	ftk_text_view_set_readonly(text_view, 1);

	button = ftk_button_create(win, width/4, 3*height/4, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "text_view demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

