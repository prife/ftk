#include "ftk_source_tslib.h"

Ret on_event(void* user_data, FtkEvent* event)
{
	printf("%s: %d %d\n", __func__, event->type, event->u.key.code);
	return RET_OK;
}

int main(int argc, char* argv[])
{
	int i = 0;
	FtkSource* thiz = NULL;
	const char* filename = argv[1] != NULL ? argv[1] : "/dev/input/event2";
	thiz = ftk_source_tslib_create(filename, on_event, NULL);
	assert(ftk_source_get_fd(thiz) > 0);
	assert(ftk_source_check(thiz) < 0);

	for(i = 0; i < 100; i++)
	{
		ftk_source_dispatch(thiz);
	}

	ftk_source_unref(thiz);

	return 0;
}
