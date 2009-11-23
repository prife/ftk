#include "ftk.h"
#include "ftk_xul.h"
#include "test_util.c"

const char* t1 = "<window> </window>";

int main(int argc, char* argv[])
{
	int length = 0;
	char* buffer = NULL;
	FtkWidget* win = NULL;
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
	ftk_widget_show_all(win, 1);

	ftk_run();

	return 0;
}
