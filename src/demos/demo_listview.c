#include "ftk.h"

#include "ftk_list_view.h"
#include "ftk_list_render_default.h"
#include "ftk_list_model_default.h"

#define IDC_TEST_BUTTON 1000
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

void item_destroy(FtkListItemInfo* info)
{

	return;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* list = NULL;
	FtkListModel* model = NULL;
	FtkListRender* render = NULL;
	FtkListItemInfo info = {0};
	ftk_init(argc, argv);
	
	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	
	list = ftk_list_view_create(win, 10, 5, width - 20, 3 * height/4-5);
	model = ftk_list_model_default_create(10, item_destroy);
	render = ftk_list_render_default_create();
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);
	ftk_list_model_default_add(model, &info);

	ftk_list_view_init(list, model, render, 50);

	button = ftk_button_create(win, width/4, 3 * height/4 + 5, width/2, 60);
	ftk_widget_set_text(button, "quit");
	ftk_button_set_clicked_listener(button, button_quit_clicked, win);
	ftk_window_set_focus(win, button);

	ftk_widget_set_text(win, "button demo");
	ftk_widget_show_all(win, 1);
	ftk_widget_set_user_data(win, on_window_close, win);

	ftk_run();

	return 0;
}

