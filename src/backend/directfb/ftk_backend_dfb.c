#include "ftk_globals.h"
#include "ftk_display_dfb.h"
#include "ftk_source_dfb.h"

#define DFBCHECK(x) x
Ret ftk_backend_init(int argc, char* argv[])
{
	IDirectFB* dfb = NULL;
	FtkSource* source = NULL;
	DFBCHECK(DirectFBInit( &argc, &argv ));
	DFBCHECK(DirectFBCreate( &dfb ));

	ftk_set_display(ftk_display_dfb_create(dfb));
	if((source = ftk_source_dfb_create(dfb)) != NULL)
	{
		ftk_sources_manager_add(ftk_default_sources_manager(), source);
	}

	return RET_OK;
}
