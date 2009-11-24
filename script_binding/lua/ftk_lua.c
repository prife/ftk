/*
 * File: ftk_lua.h    
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lua binding. 
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
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

#include "ftk.h"
#include "ftk_lua.h"
#include "lauxlib.h"

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

static const struct luaL_reg mylib [] =
{
	{"ltk_init", ltk_init},
	{"ltk_run",  ltk_run},
	{"ltk_quit", ltk_quit},
	{NULL, NULL}
};

int ftk_lua_init(lua_State *L) 
{
  const luaL_Reg *lib = mylib;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }

	return 1;
}


