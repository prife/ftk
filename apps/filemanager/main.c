#include "ftk.h"
#include "ftk_xul.h"
#include "ftk_expr.h"
#include "ftk_animator_expand.h"
#include "ftk_list_render_default.h"

typedef enum _FileManagerType
{
	FILE_MANAGER_NORMAL,
	FILE_MANAGER_SINGLE_SELECT,
	FILE_MANAGER_MULTI_SELECT
}FileManagerType;

Ret file_manager_set_path(FtkListModel* model, const char* path, FileManagerType type, const char* mime_type);

static void item_destroy(FtkListItemInfo* info)
{
	if(info != NULL)
	{
		FTK_FREE(info->text);
	}

	return;
}

typedef struct _AppInfo
{
	FtkBitmap* file;
	FtkBitmap* folder;
	FtkListModel* model;
	char cwd[FTK_MAX_PATH+1];
	FileManagerType type;
	const char* mime_type;
}AppInfo;

static AppInfo g_app_info;

static Ret on_quit(void* ctx, void* obj)
{
#ifdef HAS_MAIN
	ftk_quit();
#else
	ftk_widget_unref(ctx);
#endif
	return RET_OK;
}

static Ret on_prepare_options_menu(void* ctx, FtkWidget* menu_panel)
{
	FtkWidget* item = ftk_menu_item_create(menu_panel);
	ftk_widget_set_text(item, "Quit");
	ftk_menu_item_set_clicked_listener(item, on_quit, ctx);
	ftk_widget_show(item, 1);

	return  RET_OK;
}

Ret on_item_clicked(void* ctx, void* list)
{
    FtkListItemInfo* info = NULL;
    FtkListModel* model = ftk_list_view_get_model(list);
    int i = ftk_list_view_get_selected(list);
    
    ftk_list_model_get_data(model, i, (void**)&info);

    if(info != NULL && info->text != NULL)
    {
    	char* p = NULL;
    	char path[FTK_MAX_PATH+1] = {0};
    	char* filename = info->text;

		if(strcmp(filename, "..") == 0)
		{
			strcpy(path, g_app_info.cwd);
			if((p = strrchr(path, '/')) != NULL)
			{
				*p = '\0';
			}
			file_manager_set_path(g_app_info.model, path, g_app_info.type, g_app_info.mime_type);
		}
		else if(info->value & DT_DIR)
		{
    		snprintf(path, FTK_MAX_PATH, "%s/%s", g_app_info.cwd, filename);
			file_manager_set_path(g_app_info.model, path, g_app_info.type, g_app_info.mime_type);
    	}
    }

    return RET_OK;
}

Ret file_manager_set_path(FtkListModel* model, const char* path, FileManagerType type, const char* mime_type)
{
	DIR* dir = NULL;
	FtkListItemInfo info = {0};
	struct dirent* ent = NULL;
	return_val_if_fail(model != NULL && path != NULL, RET_FAIL);

	dir = opendir(path);
	return_val_if_fail(dir != NULL, RET_FAIL);

	strncpy(g_app_info.cwd, path, FTK_MAX_PATH);
	ftk_list_model_reset(model);
	
	if(strcmp(path, "/") != 0)
	{
		info.value = DT_DIR;
		info.text = FTK_STRDUP("..");
		info.type = FTK_LIST_ITEM_NORMAL;
		info.left_icon = g_app_info.folder;	
		ftk_list_model_add(model, &info);
	}

	while((ent = readdir(dir)) != NULL)
	{
		if(ent->d_name[0] == '.')
		{
			continue;
		}
		if(ent->d_type & DT_DIR)
		{
			info.value = ent->d_type;
			info.type = FTK_LIST_ITEM_NORMAL;
			info.text = FTK_STRDUP(ent->d_name);
			info.left_icon = g_app_info.folder;

			ftk_list_model_add(model, &info);
		}
	}

	rewinddir(dir);
	while((ent = readdir(dir)) != NULL)
	{
		if(ent->d_name[0] == '.')
		{
			continue;
		}
		if(!(ent->d_type & DT_DIR))
		{
			info.value = ent->d_type;
			info.type = FTK_LIST_ITEM_NORMAL;
			info.text = FTK_STRDUP(ent->d_name);
			info.left_icon = g_app_info.file;

			ftk_list_model_add(model, &info);
		}
	}

	closedir(dir);

	return RET_OK;
}

FtkBitmap* file_manager_load_bitmap(const char* name)
{
	FtkBitmap* bitmap = NULL;
	char filename[FTK_MAX_PATH + 1] = {0};
	snprintf(filename, FTK_MAX_PATH, "%s/icons/%s", APP_DATA_DIR, name);
	bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
	if(bitmap != NULL) return bitmap;

	snprintf(filename, FTK_MAX_PATH, "%s/icons/%s", APP_LOCAL_DATA_DIR, name);
	bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);

	return bitmap;
}

FtkWidget* file_manager_create(const char* path, FileManagerType type, const char* mime_type)
{
	FtkListModel* model = NULL;
	FtkListRender* render = NULL;	
	FtkWidget* win = ftk_app_window_create();
	FtkWidget* list = ftk_list_view_create(win, 0, 0, ftk_widget_width(win), ftk_widget_height(win));

	ftk_widget_set_text(win, "File Manager");
	ftk_app_window_set_on_prepare_options_menu(win, on_prepare_options_menu, win);
	ftk_list_view_set_clicked_listener(list, on_item_clicked, NULL);
	model = ftk_list_model_default_create(10, (FtkDestroy)item_destroy);
	render = ftk_list_render_default_create();

	g_app_info.model = model;
	g_app_info.file = file_manager_load_bitmap("file.png"); 
	g_app_info.folder = file_manager_load_bitmap("folder.png");

	file_manager_set_path(model, path, type, mime_type);

	ftk_list_view_init(list, model, render, 40);
	ftk_list_model_unref(model);

	return win;
}

#ifdef HAS_MAIN
int main(int argc, char* argv[])
#else
Ret ftk_main(int argc, char* argv[])
#endif
{
	FtkWidget* win  = NULL;
#ifdef HAS_MAIN
	ftk_init(argc, argv);
#endif
	win = file_manager_create("/", FILE_MANAGER_NORMAL, "*/*");
	ftk_widget_show_all(win, 1);

#ifdef HAS_MAIN
	ftk_widget_set_attr(win, FTK_ATTR_QUIT_WHEN_CLOSE);
	ftk_run();
#endif

	return RET_OK;
}
