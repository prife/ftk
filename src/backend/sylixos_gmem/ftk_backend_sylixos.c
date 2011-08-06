#include "ftk_display_sylixos.h"
#include "ftk_source_sylixos.h"
#include "ftk_wnd_manager.h"
#include "ftk_globals.h"

int ftk_sylixos_ts_calibrate(void);

Ret ftk_backend_init (int argc, char *argv[])
{
    FtkDisplay* display = NULL;
    FtkSource*  source = NULL;
    char        namebuffer[PATH_MAX + 1];
    char*       name = FTK_FB_NAME;

    ftk_sylixos_ts_calibrate();

    if (getenv_r("FRAMEBUFFER", namebuffer, PATH_MAX + 1) >= 0)
    {
        name = namebuffer;
    }

    display = ftk_display_sylixos_create(name);

    ftk_set_display(display);

    source = ftk_source_sylixos_input_create();
    if (source != NULL)
    {
        ftk_sources_manager_add(ftk_default_sources_manager(), source);
    }

    return display != NULL && source != NULL ? RET_OK : RET_FAIL;
}
