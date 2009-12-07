#include "ftk.h"
#include "ftk_xul.h"

const char* t1 = "<window> </window>";

#define IDC_QUIT 100

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

static FtkIconCache* g_icon_cache = NULL;
static FtkBitmap* my_load_image(const char* filename)
{
	return ftk_icon_cache_load(g_icon_cache, filename);
}

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		FtkWidget* win = NULL;
		FtkWidget* quit = NULL;
		ftk_init(argc, argv);
		
		g_icon_cache = ftk_icon_cache_create(NULL, "testdata");
		win = ftk_xul_load_file(argv[1], NULL, my_load_image);
		ftk_widget_set_user_data(win, on_window_close, win);
		
		quit = ftk_widget_lookup(win, IDC_QUIT);
		ftk_button_set_clicked_listener(quit, button_quit_clicked, win);
		ftk_widget_show_all(win, 1);

		ftk_run();
		ftk_icon_cache_destroy(g_icon_cache);
	}
	else
	{
		ftk_logd("Usage: %s xul\n", argv[0]);

		return 0;
	}

	return 0;
}
