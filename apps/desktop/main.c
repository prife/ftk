#include "ftk.h"
#include <time.h>
#include <dlfcn.h>
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

FtkIconCache* g_icon_cache = NULL;
static FtkBitmap* my_load_image(const char* filename)
{
	return ftk_icon_cache_load(g_icon_cache, filename);
}

static FtkWidget* desktop_load_xul(const char* filename)
{
	char path[FTK_MAX_PATH+1] = {0};
	
	tr_path(filename, path);

	return ftk_xul_load_file(path, NULL, my_load_image);
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
		default:
		case FTK_ANI_TO_UP:
		{
			delta = height/8;
			ftk_animator_set_param(g_animator, type, height - delta, ftk_widget_top(widget), delta, 200);
			break;
		}
	}

	ftk_animator_start(g_animator, widget, 0);

	return RET_OK;
}

static Ret app_item_clicked(void* ctx, void* obj)
{
	FtkMain entry = NULL;
	FtkIconViewItem* item = obj;
	AppInfo* info = item->user_data;
	if(info->handle == NULL)
	{
		info->handle = dlopen(info->exec, RTLD_NOW);
	}
	else
	{
		ftk_loge("%s: dlopen %s failed. %s\n", __func__, info->exec, dlerror());
	}
	
	if(info->handle != NULL)
	{
		entry = dlsym(info->handle, info->main);
		if(entry != NULL)
		{
			entry(0, NULL);
		}
		else
		{
			ftk_loge("%s: dlsync %s failed. %s\n", __func__, info->main, dlerror());
		}
	}

	ftk_logd("%s: %s: user_data=%d\n", __func__, item->text, item->user_data);

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
	ftk_icon_view_set_clicked_listener(icon_view, app_item_clicked, win);
	n = app_info_manager_get_count(g_app_manager);
	
	for(i = 0; i < n; i++)
	{
		app_info_manager_get(g_app_manager, i, &app_info);
		
		item.icon = app_info->icon_bitmap;
		item.user_data = app_info;
		strncpy(item.text, app_info->name, sizeof(item.text)-1);
		ftk_icon_view_add(icon_view, &item);
	}

	g_applist_win = win;

	return RET_OK;
}

static const char* s_default_path[FTK_ICON_PATH_NR]=
{
	FTK_DATA_ROOT"/desktop",
	".",
	NULL,
	NULL
};

static Ret update_time(void* ctx)
{
	time_t now = time(0);
	FtkWidget* win = ctx;
	FtkWidget* image = NULL;
	FtkBitmap* bitmap = NULL;
	char filename[FTK_MAX_PATH] = {0};
	struct tm* t = localtime(&now);

	image = ftk_widget_lookup(win, 1);
	snprintf(filename, sizeof(filename)-1, "icons/%d.png", t->tm_hour/10);
	bitmap = ftk_icon_cache_load(g_icon_cache, filename);
	ftk_image_set_image(image, bitmap);
	
	image = ftk_widget_lookup(win, 2);
	snprintf(filename, sizeof(filename)-1, "icons/%d.png", t->tm_hour%10);
	bitmap = ftk_icon_cache_load(g_icon_cache, filename);
	ftk_image_set_image(image, bitmap);

	image = ftk_widget_lookup(win, 4);
	snprintf(filename, sizeof(filename)-1, "icons/%d.png", t->tm_min/10);
	bitmap = ftk_icon_cache_load(g_icon_cache, filename);
	ftk_image_set_image(image, bitmap);
	
	image = ftk_widget_lookup(win, 5);
	snprintf(filename, sizeof(filename)-1, "icons/%d.png", t->tm_min%10);
	bitmap = ftk_icon_cache_load(g_icon_cache, filename);
	ftk_image_set_image(image, bitmap);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	char path[FTK_MAX_PATH] = {0};
	FtkSource* timer = NULL;
	ftk_init(argc, argv);
	
	if(argv[1] != NULL && strncmp(argv[1], "--hor", 5) == 0)
	{
		g_desktop_horizonal = 1;
	}

	g_app_manager = app_info_manager_create();
	g_animator = ftk_animator_expand_create(0);

	snprintf(path, sizeof(path), DATA_DIR"/apps");
	if(app_info_manager_load_dir(g_app_manager, path) != RET_OK)
	{
		snprintf(path, sizeof(path), LOCAL_DATA_DIR"/apps");
		app_info_manager_load_dir(g_app_manager, path);
	}

	g_icon_cache = ftk_icon_cache_create(s_default_path, NULL);
	win = desktop_load_xul(g_desktop_horizonal ? "xul/desktop-h.xul" : "xul/desktop-v.xul"); 
	button = ftk_widget_lookup(win, 100);
	ftk_button_set_clicked_listener(button, button_open_applist_clicked, win);
	ftk_widget_show_all(win, 1);

	update_time(win);
	timer = ftk_source_timer_create(60000, update_time, win);
	ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	ftk_run();

	ftk_icon_cache_destroy(g_icon_cache);
	app_info_manager_destroy(g_app_manager);
	ftk_animator_destroy(g_animator);

	return 0;
}
