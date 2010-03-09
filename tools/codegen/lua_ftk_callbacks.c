#include <ftk.h>

Ret lua_ftk_prepare_options_menu_func(void* ctx, FtkWidget* menu_panel)
{
	return RET_OK;
}

Ret lua_ftk_on_event_func(void* user_data, FtkEvent* event)
{
	printf("%s:%d\n", __func__, __LINE__);

	return RET_OK;
}

Ret  lau_ftk_widget_on_event_func(FtkWidget* thiz, FtkEvent* event)
{
	printf("%s:%d\n", __func__, __LINE__);

	return RET_OK;
}

void lua_ftk_destroy_func(void* user_data)
{
	printf("%s:%d\n", __func__, __LINE__);

	return;
}

Ret lua_ftk_idle_func(void* user_data)
{
	printf("%s:%d\n", __func__, __LINE__);
	return RET_OK;
}

Ret lua_ftk_timer_func(void* user_data)
{
	printf("%s:%d\n", __func__, __LINE__);
	return RET_OK;
}

int lua_ftk_compare_func(const void* obj1, const void* obj2)
{
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}

Ret lua_ftk_listener_func(void* user_data, void* obj)
{
	printf("%s:%d\n", __func__, __LINE__);

	return RET_OK;
}

