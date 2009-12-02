#include "ftk.h"

int main(int argc, char* argv[])
{
	FtkWidget* win = NULL;

	ftk_init(argc, argv);
	win = ftk_app_window_create();
	ftk_widget_set_text(win, "Hello FTK!");
	ftk_widget_show(win, 1);

	ftk_run();

	return 0;
}

