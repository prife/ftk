#include "ftk.h"

int main(int argc, char* argv[])
{
	FtkWidget* win = NULL;

	FTK_INIT(argc, argv);
	win = ftk_app_window_create();
	ftk_window_set_animation_hint(win, "app_main_window");
	ftk_widget_set_text(win, "Hello FTK!");
	ftk_widget_show(win, 1);
	FTK_QUIT_WHEN_WIDGET_CLOSE(win);

	FTK_RUN();

	return 0;
}

