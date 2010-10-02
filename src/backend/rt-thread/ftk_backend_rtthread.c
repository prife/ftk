
#include "ftk_log.h"
#include "ftk_globals.h"
#include "ftk_display_rtthread.h"

Ret ftk_backend_init(int argc, char* argv[])
{
    ftk_set_display(ftk_display_lcd_create("/dev/lcd"));

	ftk_default_display();

    return RET_OK;
}
