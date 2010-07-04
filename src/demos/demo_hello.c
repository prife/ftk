#include "ftk.h"

int FTK_MAIN(int argc, char* argv[])
{
	FtkWidget* win = NULL;

	FTK_INIT(argc, argv);
	win = ftk_app_window_create();
	ftk_widget_set_text(win, "Hello FTK!");
	ftk_widget_show(win, 1);
	FTK_QUIT_WHEN_WIDGET_CLOSE(win);

	FTK_RUN();

	return 0;
}

