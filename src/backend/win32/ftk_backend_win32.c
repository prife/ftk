#include "ftk_display_win32.h"
#include "ftk_source_win32.h"
#include "ftk_wnd_manager.h"
#include "ftk_globals.h"

Ret ftk_backend_init(void)
{
	FtkSource* source = NULL;
	FtkDisplay* display = NULL;
#if 0
	display = ftk_display_win32_create(&source, (FtkOnEvent)ftk_wnd_manager_queue_event, ftk_default_wnd_manager());
	ftk_set_display(display);
	ftk_sources_manager_add(ftk_default_sources_manager(), source);
#endif
	return display != NULL ? RET_OK : RET_FAIL;
}
