
#include "ftk_log.h"
#include "ftk_globals.h"
#include "ftk_display_rtthread.h"
#include "ftk_source_touch.h"

static void ftk_source_touch_init(void)
{
    FtkSource* source = NULL;

    source = ftk_source_touch_create(FTK_TOUCH_NAME,
                (FtkOnEvent)ftk_wnd_manager_queue_event_auto_rotate, ftk_default_wnd_manager());
    if (source != NULL)
    {
        ftk_sources_manager_add(ftk_default_sources_manager(), source);
    }
    ftk_logd("%s: touch dev %s source=%p\n", __func__, FTK_TOUCH_NAME, source);
}

Ret ftk_backend_init(int argc, char* argv[])
{
    ftk_set_display(ftk_display_lcd_create("/dev/lcd"));

	ftk_default_display();

	ftk_source_touch_init();

    return RET_OK;
}
