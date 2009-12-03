#include "ftk_log.h"
#include "ftk_globals.h"
#include "ftk_display_fb.h"
#include "ftk_source_input.h"
#include "ftk_wnd_manager.h"
#include "ftk_source_tslib.h"

static Ret ftk_init_input(void)
{
	char filename[260] = {0};
	struct dirent* iter = NULL;
	DIR* dir = opendir("/dev/input");
	
	return_val_if_fail(dir != NULL, RET_FAIL);

	while((iter = readdir(dir)) != NULL)
	{
		FtkSource* source = NULL;

		if(iter->d_name[0] == '.') continue;
		if(!(iter->d_type & DT_CHR)) continue;

		ftk_snprintf(filename, sizeof(filename), "/dev/input/%s", iter->d_name);
#ifdef USE_TSLIB
		if(strcmp(filename, FTK_TSLIB_FILE) == 0)
		{
			source = ftk_source_tslib_create(filename, 
				(FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
		}
		else
#endif
		{
			source = ftk_source_input_create(filename, 
				(FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
		}
		if(source != NULL)
		{
			ftk_sources_manager_add(ftk_default_sources_manager(), source);
		}
	}
	closedir(dir);

	return RET_OK;
}

Ret ftk_backend_init(int argc, char* argv[])
{
	ftk_init_input();
	ftk_set_display(ftk_display_fb_create(FTK_FB_NAME));
	if(ftk_default_display() == NULL)
	{
		ftk_loge("open display failed.\n");
		exit(0);
	}

	return RET_OK;
}

