#include "ftk.h"

static Ret button_quit_clicked(void* ctx, void* obj)
{
	*(int*)ctx = ftk_widget_id(obj);

	return RET_QUIT;
}

static void on_window_close(void* user_data)
{
	FtkEvent event = {0};
	ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &event);

	return ;
}

int main(int argc, char* argv[])
{
	int id = 0;
	int width = 0;
	int height = 0;
	FtkWidget* label = NULL;
	FtkWidget* button = NULL;
	FtkWidget* dialog = NULL;
	FtkBitmap* icon = NULL;

	ftk_init(argc, argv);
	
	dialog = ftk_dialog_create(0, 40, 320, 240);
	
	icon = ftk_theme_load_image(ftk_default_theme(), "info-icon"FTK_STOCK_IMG_SUFFIX); 
	ftk_dialog_set_icon(dialog, icon);

	width = ftk_widget_width(dialog);
	height = ftk_widget_height(dialog);
	label = ftk_label_create(dialog, width/6, height/4, 5*width/6, 20);
	ftk_widget_set_text(label, "Are you sure to quit?");

	button = ftk_button_create(dialog, width/6, height/2, width/3, 50);
	ftk_widget_set_text(button, "yes");
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	
	button = ftk_button_create(dialog, width/2, height/2, width/3, 50);
	ftk_widget_set_text(button, "no");
	ftk_button_set_clicked_listener(button, button_quit_clicked, &id);
	ftk_window_set_focus(dialog, button);

	ftk_widget_set_text(dialog, "dialog demo");
	ftk_widget_show_all(dialog, 1);
	ftk_widget_set_user_data(dialog, on_window_close, dialog);

	assert(ftk_dialog_run(dialog) == id);

	return 0;
}

