#include "ftk.h"
#include "ftk_xul.h"

const char* t1 = "<window> </window>";

#define IDC_QUIT 100

static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

static FtkIconCache* g_icon_cache = NULL;
static FtkBitmap* my_load_image(const char* filename)
{
	return ftk_icon_cache_load(g_icon_cache, filename);
}

int FTK_MAIN(int argc, char* argv[])
{
	if(argc > 1)
	{
		FtkWidget* win = NULL;
		FtkWidget* quit = NULL;
		ftk_init(argc, argv);
		
		g_icon_cache = ftk_icon_cache_create(NULL, "testdata");
		win = ftk_xul_load_file(argv[1], NULL, my_load_image);
		ftk_widget_set_attr(win, FTK_ATTR_QUIT_WHEN_CLOSE);

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
