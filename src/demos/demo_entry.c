#include "ftk.h"

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

int main(int argc, char* argv[])
{
	ftk_init(argc, argv);
	
	FtkWidget* win = ftk_window_create(0, 2, 320, 478);

	FtkWidget* entry = ftk_entry_create(win, 10, 30, ftk_widget_width(win) - 20, 30);
	ftk_entry_set_text(entry, "Single line editor");

	entry = ftk_entry_create(win, 10, 80, ftk_widget_width(win) - 20, 30);
	ftk_entry_set_text(entry, "Single line editor, that means you can input a one line only.");
	
	entry = ftk_entry_create(win, 10, 130, ftk_widget_width(win) - 20, 30);
	ftk_entry_set_text(entry, "Single line editor, 也就是说你只能输入一行文字.");

	ftk_widget_set_text(win, "entry demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

