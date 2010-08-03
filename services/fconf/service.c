#include "fconf_service.h"

int main(int argc, char* argv[])
{
	fbus_service_init(argc, argv);
	
	if(fconf_service_create() != NULL)
	{
		fbus_service_run();
	}
	else
	{
		ftk_loge("Create service echo failed.\n");
	}

	return 0;
}
