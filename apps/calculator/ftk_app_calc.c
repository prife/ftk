#include "ftk_xul.h"
#include "ftk_expr.h"
#include "ftk_app_calc.h"
#include "ftk_animator_expand.h"

typedef struct _PrivInfo
{
	FtkBitmap* icon;
}PrivInfo;

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

FtkIconCache* g_icon_cache = NULL;
static FtkBitmap* my_load_image(const char* filename)
{
	return ftk_icon_cache_load(g_icon_cache, filename);
}

static FtkWidget* load_xul(const char* filename)
{
	char path[FTK_MAX_PATH+1] = {0};
	
	ftk_translate_path(filename, path);
	return ftk_xul_load_file(path, ftk_translate_text, my_load_image);
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

static const char* s_default_path[FTK_ICON_PATH_NR]=
{
	FTK_DATA_ROOT"/calc",
	".",
	"",
	""
};

static FtkBitmap* ftk_app_calc_get_icon(FtkApp* thiz)
{
	DECL_PRIV(thiz, priv);
	const char* name="calc.png";
	char file_name[FTK_MAX_PATH + 1] = {0};
	return_val_if_fail(priv != NULL, NULL);
	
	if(priv->icon != NULL) return priv->icon;
	
	snprintf(file_name, FTK_MAX_PATH, "%s/icons/%s", APP_DATA_DIR, name);
	priv->icon = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), file_name);
	if(priv->icon != NULL) return priv->icon;

	snprintf(file_name, FTK_MAX_PATH, "%s/icons/%s", APP_LOCAL_DATA_DIR, name);
	priv->icon = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), file_name);

	return priv->icon;
}

static const char* ftk_app_calc_get_name(FtkApp* thiz)
{
	return _("Calculator");
}

static Ret ftk_app_calc_run(FtkApp* thiz, int argc, char* argv[])
{
	int i = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	g_icon_cache = ftk_icon_cache_create(s_default_path, NULL);
	win = load_xul("xul/calc.xul"); 
	ftk_icon_cache_destroy(g_icon_cache);

	for(i = 1; i <= 20; i++)
	{
		if((button = ftk_widget_lookup(win, i)) != NULL)
		{
			ftk_button_set_clicked_listener(button, button_clicked, win);
		}
	}
	ftk_widget_show_all(win, 1);
#ifdef HAS_MAIN
	FTK_QUIT_WHEN_WIDGET_CLOSE(win);
#endif	
	return RET_OK;
}

static void ftk_app_calc_destroy(FtkApp* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		ftk_bitmap_unref(priv->icon);
		FTK_FREE(thiz);
	}

	return;
}

FtkApp* ftk_app_calc_create(void)
{
	FtkApp* thiz = FTK_ZALLOC(sizeof(FtkApp) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->run  = ftk_app_calc_run;
		thiz->get_icon = ftk_app_calc_get_icon;
		thiz->get_name = ftk_app_calc_get_name;
		thiz->destroy = ftk_app_calc_destroy;
	}

	return thiz;
}
