#include "ftk_display_sylixos.h"
#include "ftk_source_sylixos.h"
#include "ftk_wnd_manager.h"
#include "ftk_globals.h"

Ret ftk_backend_init (int argc, char *argv[])
{
    FtkDisplay* display = NULL;
    char         namebuffer[PATH_MAX + 1];
    char        *name = FTK_FB_NAME;

    if (getenv_r("FRAMEBUFFER", namebuffer, PATH_MAX + 1) >= 0) {
        name = namebuffer;
    }

    display = ftk_display_sylixos_create(name);

    ftk_set_display(display);

    ftk_source_sylixos_input_create();

    return display != NULL ? RET_OK : RET_FAIL;
}
