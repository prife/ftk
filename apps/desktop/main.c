#include "ftk.h"
#include "ftk_xul.h"
#include "ftk_animator_expand.h"

static int g_desktop_horizonal = 0;

static Ret button_close_applist_clicked(void* ctx, void* obj)
{
	ftk_widget_unref(ctx);

	return RET_OK;
}

const char* tr_path(const char* path, char out_path[FTK_MAX_PATH+1])
{
	snprintf(out_path, FTK_MAX_PATH, "%s/%s", DESKTOP_DATA_DIR, path);
	if(access(out_path, R_OK) < 0)
	{
		snprintf(out_path, FTK_MAX_PATH, "%s/%s", DESKTOP_LOCAL_DATA_DIR, path);
	}

	return out_path;
}


static FtkWidget* desktop_load_xul(const char* filename)
{
	char path[FTK_MAX_PATH+1] = {0};
	
	tr_path(filename, path);
	return ftk_xul_load_file(filename, NULL, tr_path);
}

static Ret button_open_applist_clicked(void* ctx, void* obj)
{
	FtkWidget* button = NULL;
	FtkWidget* win = NULL;

	win = desktop_load_xul(g_desktop_horizonal ? "xul/appview-h.xul" : "xul/appview-v.xul"); 
	button = ftk_widget_lookup(win, 100);
	ftk_button_set_clicked_listener(button, button_close_applist_clicked, win);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	ftk_init(argc, argv);
	
	if(argv[1] != NULL && strncmp(argv[1], "--hor", 5) == 0)
	{
		g_desktop_horizonal = 1;
	}

	win = desktop_load_xul(g_desktop_horizonal ? "xul/desktop-h.xul" : "xul/desktop-v.xul"); 
	button = ftk_widget_lookup(win, 100);
	ftk_button_set_clicked_listener(button, button_open_applist_clicked, win);
	ftk_widget_show_all(win, 1);

	ftk_run();

	return 0;
}
