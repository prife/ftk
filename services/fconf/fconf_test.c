#include "fconf_client.h"

int main(int argc, char* argv[])
{
	Ret ret = RET_OK;
	char* value = NULL;
#ifndef USE_STD_MALLOC
	ftk_set_allocator((ftk_allocator_default_create()));
#endif
	FConf* fconf = fconf_client_create();

	ret = fconf_set(fconf, "/globals/apps/test", "none");
	ret = fconf_get(fconf, "/globals/apps/test", &value);

	fconf_destroy(fconf);

	return 0;
}
