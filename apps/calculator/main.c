#include "ftk.h"
#include "ftk_xul.h"
#include "ftk_expr.h"
#include "ftk_animator_expand.h"

const char* ftk_translate_text(const char* text)
{
	return text;
}

const char* ftk_translate_path(const char* path, char out_path[FTK_MAX_PATH+1])
{
	snprintf(out_path, FTK_MAX_PATH, "%s/%s", APP_DATA_DIR, path);
	if(access(out_path, R_OK) < 0)
	{
		snprintf(out_path, FTK_MAX_PATH, "%s/%s", APP_LOCAL_DATA_DIR, path);
	}
	ftk_logd("%s->%s\n", path, out_path);
	return out_path;
}

static FtkWidget* load_xul(const char* filename)
{
	char path[FTK_MAX_PATH+1] = {0};
	
	ftk_translate_path(filename, path);
	return ftk_xul_load_file(path, ftk_translate_text, ftk_translate_path);
}

static Ret button_clicked(void* ctx, void* obj)
{
	FtkWidget* entry = ftk_widget_lookup(ctx, 100);
	const char* text = ftk_widget_get_text(obj);
	return_val_if_fail(text != NULL && entry != NULL, RET_FAIL);
	ftk_logd("%s: %s\n", __func__, text);
	if(text[0] == '=')
	{
		char buff[32] = {0};
		double val = ftk_expr_eval(ftk_entry_get_text(entry));
		snprintf(buff, sizeof(buff), "%lf", val);
		ftk_entry_set_text(entry, buff);
	}
	else if(text[0] == '<')
	{
		ftk_entry_set_text(entry, "");
	}
	else
	{
		ftk_entry_insert_text(entry, -1, text);
	}

	return RET_OK;
}

static void on_window_close(void* user_data)
{
	ftk_quit();

	return ;
}

#ifdef HAS_MAIN
int main(int argc, char* argv[])
#else
Ret ftk_main(int argc, char* argv[])
#endif
{
	int i = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
#ifdef HAS_MAIN
	ftk_init(argc, argv);
#endif
	
	win = load_xul("xul/calc.xul"); 
	for(i = 1; i <= 20; i++)
	{
		if((button = ftk_widget_lookup(win, i)) != NULL)
		{
			ftk_button_set_clicked_listener(button, button_clicked, win);
		}
	}
	ftk_widget_show_all(win, 1);

#ifdef HAS_MAIN
	ftk_widget_set_user_data(win, on_window_close, win);
	ftk_run();
#endif

	return RET_OK;
}
