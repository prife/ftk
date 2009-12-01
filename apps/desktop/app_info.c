
#include "app_info.h"

struct _AppInfoManager
{
	size_t nr;
	size_t alloc_nr;
	AppInfo* infos;
};

AppInfoManager* app_info_manager_create(void)
{
	AppInfoManager* thiz = FTK_ZALLOC(sizeof(AppInfoManager));

	return thiz;
}

Ret  app_info_manager_load_file(AppInfoManager* thiz, const char* filename)
{
	return_val_if_fail(thiz != NULL && filename != NULL, RET_FAIL);

	return RET_OK;
}

Ret  app_info_manager_load_dir(AppInfoManager* thiz, const char* path)
{
	return_val_if_fail(thiz != NULL && path != NULL, RET_FAIL);

	return RET_OK;
}

int  app_info_manager_get_count(AppInfoManager* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return 0;
}

Ret  app_info_manager_get(AppInfoManager* thiz, AppInfo** info)
{
	return_val_if_fail(thiz != NULL && info != NULL, RET_FAIL);

	return RET_OK;
}

Ret  app_info_manager_add(AppInfoManager* thiz, AppInfo* info)
{
	return_val_if_fail(thiz != NULL && info != NULL, RET_FAIL);

	return RET_OK;
}

Ret  app_info_manager_remove(AppInfoManager* thiz, size_t index)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	return RET_OK;
}

void app_info_manager_destroy(AppInfoManager* thiz)
{
	size_t i = 0;
	if(thiz != NULL)
	{
		for(i = 0; i < thiz->nr; i++)
		{
			FTK_FREE(thiz->infos[i].name);
			FTK_FREE(thiz->infos[i].icon);
		}

		FTK_FREE(thiz);
	}
}


