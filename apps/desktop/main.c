#include "ftk.h"
#include "ftk_xul.h"
#include "app_info.h"
#include "ftk_animator_expand.h"

static int g_desktop_horizonal = 0;
static FtkAnimator* g_animator = NULL;
static FtkWidget* g_applist_win = NULL;
static AppInfoManager* g_app_manager = NULL;

static Ret button_close_applist_clicked(void* ctx, void* obj)
{
	ftk_widget_show(ctx, 0);

	return RET_OK;
}

const char* tr_path(const char* path, char out_path[FTK_MAX_PATH+1])
{
	struct stat st = {0};
	snprintf(out_path, FTK_MAX_PATH, "%s/%s", APP_DATA_DIR, path);

	if(stat(out_path, &st) < 0)
	{
		snprintf(out_path, FTK_MAX_PATH, "%s/%s", APP_LOCAL_DATA_DIR, path);
	}
	ftk_logd("%s: %s --> %s\n", __func__, path, out_path);

	return out_path;
}


static FtkWidget* desktop_load_xul(const char* filename)
{
	char path[FTK_MAX_PATH+1] = {0};
	
	tr_path(filename, path);

	return ftk_xul_load_file(path, NULL, tr_path);
}

static Ret applist_window_show(FtkWidget* widget)
{
	int delta = 0;
	int width = ftk_widget_width(widget);
	int height = ftk_widget_height(widget);
	int type = g_desktop_horizonal ? FTK_ANI_TO_RIGHT : FTK_ANI_TO_UP;
	
	switch(type)
	{
		case FTK_ANI_TO_RIGHT:
		{
			delta = width/8;
			ftk_animator_set_param(g_animator, type, delta, width, delta, 200);
			break;
		}
		case FTK_ANI_TO_UP:
		{
			delta = height/8;
			ftk_animator_set_param(g_animator, type, height - delta, ftk_widget_top(widget), delta, 200);
			break;
		}
		default:break;
	}

	ftk_animator_start(g_animator, widget, 0);

	return RET_OK;
}

static Ret button_open_applist_clicked(void* ctx, void* obj)
{
	size_t i = 0;
	size_t n = 0;
	FtkWidget* win = NULL;
	AppInfo* app_info = NULL;
	FtkWidget* button = NULL;
	FtkIconViewItem item;
	FtkWidget* icon_view = NULL;
	
	if(g_applist_win != NULL)
	{
		applist_window_show(g_applist_win);

		return RET_OK;
	}

	win = desktop_load_xul(g_desktop_horizonal ? "xul/appview-h.xul" : "xul/appview-v.xul"); 
	
	button = ftk_widget_lookup(win, 100);
	ftk_button_set_clicked_listener(button, button_close_applist_clicked, win);

	icon_view = ftk_widget_lookup(win, 99);
	n = app_info_manager_get_count(g_app_manager);
	
	for(i = 0; i < n; i++)
	{
		app_info_manager_get(g_app_manager, i, &app_info);
		
		item.icon = app_info->icon_bitmap;
		item.user_data = app_info;
		strncpy(item.text, app_info->name, sizeof(item.text));
		ftk_logd("%s: %s\n", __func__, item.text);
		ftk_icon_view_add(icon_view, &item);
	}

	g_applist_win = win;

	return RET_OK;
}

int main(int argc, char* argv[])
{
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	char path[FTK_MAX_PATH] = {0};
	
	ftk_init(argc, argv);
	
	if(argv[1] != NULL && strncmp(argv[1], "--hor", 5) == 0)
	{
		g_desktop_horizonal = 1;
	}

	g_app_manager = app_info_manager_create();
	g_animator = ftk_animator_expand_create();

	snprintf(path, sizeof(path), DATA_DIR"/apps");
	if(app_info_manager_load_dir(g_app_manager, path) != RET_OK)
	{
		snprintf(path, sizeof(path), LOCAL_DATA_DIR"/apps");
		app_info_manager_load_dir(g_app_manager, path);
	}

	win = desktop_load_xul(g_desktop_horizonal ? "xul/desktop-h.xul" : "xul/desktop-v.xul"); 
	button = ftk_widget_lookup(win, 100);
	ftk_button_set_clicked_listener(button, button_open_applist_clicked, win);
	ftk_widget_show_all(win, 1);

	ftk_run();

	app_info_manager_destroy(g_app_manager);
	ftk_animator_destroy(g_animator);

	return 0;
}
