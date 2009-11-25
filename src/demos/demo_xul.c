#include "ftk.h"
#include "ftk_xul.h"
#include "test_util.c"

const char* t1 = "<window> </window>";

#define IDC_QUIT 100

const char* tr_path(const char* path, char out_path[FTK_MAX_PATH+1])
{
	snprintf(out_path, FTK_MAX_PATH, "%s/%s", TESTDATA_DIR, path);

	return out_path;
}

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

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		int length = 0;
		char* buffer = NULL;
		FtkWidget* win = NULL;
		FtkWidget* quit = NULL;
		ftk_init(argc, argv);

		buffer = read_file(argv[1], &length);
		win = ftk_xul_load_ex(buffer, length, NULL, tr_path);
		free(buffer);
		ftk_widget_set_user_data(win, on_window_close, win);
		
		quit = ftk_widget_lookup(win, IDC_QUIT);
		ftk_button_set_clicked_listener(quit, button_quit_clicked, win);
		ftk_widget_show_all(win, 1);

		ftk_run();
	}
	else
	{
		ftk_logd("Usage: %s xul\n", argv[0]);

		return 0;
	}

	return 0;
}
