/*
 * File:   lua_runner.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lua runner
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

#include "ftk_lua.h"

const char LUA_SCRIPT[] = 
"ftk_init()"
"ftk_run()";

int main(int argc, char* argv[])
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	ftk_lua_init(L);
	luaL_dostring(L, LUA_SCRIPT);
	lua_close(L);
	return 0;
}
