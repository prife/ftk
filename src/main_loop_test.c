#include "ftk_main_loop.h"
#include "ftk_source_idle.h"
#include "ftk_source_timer.h"

Ret timeout_quit(void* user_data)
{
	FtkMainLoop* thiz = (FtkMainLoop*)user_data;

	ftk_main_loop_quit(thiz);

	printf("%s:%d\n", __func__, __LINE__);
	return RET_REMOVE;
}

Ret timeout_repeat(void* user_data)
{
	printf("%s:%d\n", __func__, __LINE__);
	return RET_OK;
}

Ret idle(void* user_data)
{
	printf("%s:%d\n", __func__, __LINE__);
	return RET_REMOVE;
}

int main(int argc, char* argv[])
{
	FtkSource* source = NULL;
	FtkMainLoop* thiz = ftk_main_loop_create();

	source = ftk_source_idle_create(idle, NULL);
	ftk_main_loop_add_source(thiz, source);

	source = ftk_source_timer_create(1000, timeout_repeat, thiz);
	ftk_main_loop_add_source(thiz, source);

	source = ftk_source_timer_create(10000, timeout_quit, thiz);
	ftk_main_loop_add_source(thiz, source);

	ftk_main_loop_run(thiz);
	fflush(stdout);
	ftk_main_loop_destroy(thiz);

	return 0;
}

