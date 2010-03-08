/*
 * File: ftk_lua.h    
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lua binding. 
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "ftk.h"
#include "ftk_xul.h"
#include "ftk_lua.h"

static int ltk_init(lua_State *L)
{
	lua_pushinteger(L, ftk_init(0, NULL));

	return 1;
}

static int ltk_run(lua_State *L)
{
	lua_pushinteger(L, ftk_run());

	return 1;
}

static int ltk_quit(lua_State *L)
{
	ftk_quit();

	return 1;
}

static int ltk_xul_load(lua_State *L)
{
	size_t len = 0;
	const char* xml = luaL_checklstring(L, 1, &len);
	FtkWidget* widget = ftk_xul_load(xml, len);

	lua_pushlightuserdata(L, widget);

	return 1;
}

static int ltk_widget_show_all(lua_State *L)
{
	FtkWidget* widget = lua_touserdata(L, 1);
	int visible = lua_tointeger(L, 2);

	ftk_widget_show_all(widget, visible);

	return 1;
}

static lua_State* s_current_L = NULL;
static Ret ltk_listener(void* user_data, void* obj)
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

static int ltk_button_set_clicked_listener(lua_State *L)
{
	size_t len = 0;
	Ret ret = RET_OK;
	FtkWidget* button  = lua_touserdata(L, 1);
	const char* func = luaL_checklstring(L, 2, &len); 
	ret = ftk_button_set_clicked_listener(button, ltk_listener, (void*)func);
	lua_pushinteger(L, ret);
	
	return 1;
}

static int ltk_widget_lookup(lua_State *L)
{
	FtkWidget* widget  = lua_touserdata(L, 1);
	int id = lua_tointeger(L, 2);
	
	widget = ftk_widget_lookup(widget, id);

	lua_pushlightuserdata(L, widget);

	return 1;
}

static int ltk_pass_strings(lua_State *L)
{
	int n1 = lua_tonumber(L, 1);
	char** array = tolua_tostrings(L, 2, NULL);
	int n2 = lua_tonumber(L, 3);
	char* str = lua_tostring(L, 4);
	free(array);
	printf("n1=%d n2=%d str=%s\n", n1, n2, str);
	return 1;
}

static const struct luaL_reg mylib [] =
{
	{"ltk_pass_strings",               ltk_pass_strings},
	{"ftk_init",               ltk_init},
	{"ftk_run",                ltk_run},
	{"ftk_quit",               ltk_quit},
	{"ftk_xul_load",           ltk_xul_load},
	{"ftk_widget_lookup",      ltk_widget_lookup},
	{"ftk_widget_show_all",    ltk_widget_show_all},
	{"ftk_button_set_clicked_listener", ltk_button_set_clicked_listener},

	{NULL, NULL}
};

int ftk_lua_init(lua_State *L) 
{
	const luaL_Reg *lib = mylib;
	
	for (; lib->func; lib++) 
	{
		lua_register(L, lib->name, lib->func);
	}

	s_current_L = L;

	return 1;
}


