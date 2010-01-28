#include "ftk.h"

#define IDC_TEST_BUTTON 1000
static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* combo_box = NULL;

	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	width = width/2 - 10;
	combo_box = ftk_combo_box_create(win, 0, height/4, width, 50);
	ftk_combo_box_set_text(combo_box, "1 second");
	ftk_combo_box_append(combo_box, NULL, "1 second");
	ftk_combo_box_append(combo_box, NULL, "2 seconds");
	ftk_combo_box_append(combo_box, NULL, "3 seconds");
	combo_box = ftk_combo_box_create(win, width + 10, height/4, width, 50);
	ftk_combo_box_set_text(combo_box, "1 second");
	ftk_combo_box_append(combo_box, NULL, "1 second");
	ftk_combo_box_append(combo_box, NULL, "2 seconds");
	ftk_combo_box_append(combo_box, NULL, "3 seconds");
	ftk_combo_box_append(combo_box, NULL, "4 seconds");
	ftk_combo_box_append(combo_box, NULL, "5 seconds");
	ftk_combo_box_append(combo_box, NULL, "6 seconds");
	ftk_combo_box_append(combo_box, NULL, "7 seconds");
	ftk_combo_box_append(combo_box, NULL, "8 seconds");
	ftk_combo_box_append(combo_box, NULL, "9 seconds");
	ftk_combo_box_append(combo_box, NULL, "0 seconds");
	ftk_combo_box_append(combo_box, NULL, "0 seconds");

	combo_box = ftk_combo_box_create(win, 0, 3*height/4+5, width, 50);
	ftk_combo_box_set_text(combo_box, "1 second");
	ftk_combo_box_append(combo_box, NULL, "1 second");
	ftk_combo_box_append(combo_box, NULL, "2 seconds");
	ftk_combo_box_append(combo_box, NULL, "3 seconds");
	combo_box = ftk_combo_box_create(win, width + 10, 3*height/4+5, width, 50);
	ftk_combo_box_set_text(combo_box, "1 second");
	ftk_combo_box_append(combo_box, NULL, "1 second");
	ftk_combo_box_append(combo_box, NULL, "2 seconds");
	ftk_combo_box_append(combo_box, NULL, "3 seconds");
	ftk_combo_box_append(combo_box, NULL, "4 seconds");
	ftk_combo_box_append(combo_box, NULL, "5 seconds");
	ftk_combo_box_append(combo_box, NULL, "6 seconds");
	ftk_combo_box_append(combo_box, NULL, "7 seconds");
	ftk_combo_box_append(combo_box, NULL, "8 seconds");
	ftk_combo_box_append(combo_box, NULL, "9 seconds");
	ftk_combo_box_append(combo_box, NULL, "0 seconds");
	ftk_combo_box_append(combo_box, NULL, "0 seconds");
	
	button = ftk_button_create(win, width/2, height/2, width, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "ComboBox Demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_attr(win, FTK_ATTR_QUIT_WHEN_CLOSE);

	ftk_run();

	return 0;
}

