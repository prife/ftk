#ifndef APP_INFO_H
#define APP_INFO_H

#include "ftk.h"
#include "ftk_xml_parser.h"

typedef Ret (*FtkMain)(int argc, char* argv[]);

typedef struct _AppInfo
{
	char name[32];
	char icon[32];
	char exec[260];
	int shortcut;
	char main[64];
	char tr_path[64];
	char tr_text[64];
	void* handle;
}AppInfo;

struct _AppInfoManager;
typedef struct _AppInfoManager AppInfoManager;

AppInfoManager* app_info_manager_create(void);
Ret  app_info_manager_load_file(AppInfoManager* thiz, const char* filename);
Ret  app_info_manager_load_dir(AppInfoManager* thiz, const char* path);
int  app_info_manager_get_count(AppInfoManager* thiz);
Ret  app_info_manager_get(AppInfoManager* thiz, size_t index, AppInfo** info);
Ret  app_info_manager_add(AppInfoManager* thiz, AppInfo* info);
Ret  app_info_manager_remove(AppInfoManager* thiz, size_t index);
void app_info_manager_destroy(AppInfoManager* thiz);

#endif/*APP_INFO_H*/

