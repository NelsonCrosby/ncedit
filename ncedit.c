/**************************************
 * The MIT License (MIT)
 * Copyright (c) 2016, Nelson Crosby <nelsonc@sourcecomb.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *************************************/


#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


int main(int argc, char** argv) {

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadfile(L, "lua/main.lua")) {
        printf("cannot load main.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_createtable(L, argc - 1, 1);
    lua_pushstring(L, "argv0");
    lua_pushstring(L, argv[0]);
    lua_settable(L, -3);
    for (int i = 0; i < argc; i += 1) {
        lua_pushnumber(L, (lua_Number) i);
        lua_pushstring(L, argv[i]);
        lua_settable(L, -3);
    }

    if (lua_pcall(L, 1, 1, 0)) {
        printf("error running main.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}
