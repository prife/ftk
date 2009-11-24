#include "ftk.h"
#include "ftk_xul.h"
#include "test_util.c"

const char* t1 = "<window> </window>";

#define IDC_QUIT 100
static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_quit();

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int length = 0;
	char* buffer = NULL;
	FtkWidget* win = NULL;
	FtkWidget* quit = NULL;
	ftk_init(argc, argv);

	if(argc > 1)
	{
		buffer = read_file(argv[1], &length);
		win = ftk_xul_load(buffer, length);
		free(buffer);
	}
	else
	{
		win = ftk_xul_load(t1, strlen(t1));
	}
	quit = ftk_widget_lookup(win, IDC_QUIT);
	ftk_button_set_clicked_listener(quit, button_quit_clicked, win);
	ftk_widget_show_all(win, 1);

	ftk_run();

	return 0;
}
