#include <ftk.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static lua_State* s_current_L = NULL;

void lua_callbacks_init(lua_State* L)
{
	s_current_L = L;

	return;
}

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
    Ret ret = RET_OK;
    const char* func = user_data;
    lua_State *L = s_current_L;

    lua_getglobal(L, func);
    lua_call(L, 0, 1);
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

	printf("%s:%d\n", __func__, __LINE__);
    return ret;
}

Ret lua_ftk_timer_func(void* user_data)
{
    Ret ret = RET_OK;
    const char* func = user_data;
    lua_State *L = s_current_L;

    lua_getglobal(L, func);
    lua_call(L, 0, 1);
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

	printf("%s:%d ret=%d\n", __func__, __LINE__, ret);
    return ret;
}

int lua_ftk_compare_func(const void* obj1, const void* obj2)
{
	printf("%s:%d\n", __func__, __LINE__);
	return 0;
}

Ret lua_ftk_listener_func(void* user_data, void* obj)
{
    Ret ret = RET_OK;
    const char* func = user_data;
    lua_State *L = s_current_L;

    lua_getglobal(L, func);
    lua_pushlightuserdata(L, obj);
    lua_call(L, 1, 1);
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return ret;
}

Ret lua_ftk_list_item_listener_func(void* user_data, void* obj)
{
    Ret ret = RET_OK;
    const char* func = user_data;
    lua_State *L = s_current_L;

    lua_getglobal(L, func);
	tolua_pushusertype(L, obj, "FtkListItemInfo");
    lua_call(L, 1, 1);
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return ret;
}

Ret lua_ftk_icon_view_item_listener_func(void* user_data, void* obj)
{
    Ret ret = RET_OK;
    const char* func = user_data;
    lua_State *L = s_current_L;

    lua_getglobal(L, func);
	tolua_pushusertype(L, obj, "FtkIconViewItem");
    lua_call(L, 1, 1);
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return ret;
}
