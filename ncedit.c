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

#include "lapi/lapi.h"


// Function for getting error stack trace
static int traceback(lua_State *L) {
    // Only get traceback if message is a string
    if (!lua_isstring(L, 1))
        return 1;
    // Get stack trace for L
    luaL_traceback(L, L, lua_tostring(L, -1), 1);
    return 1;
}


int main(int argc, char **argv)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lapi_load(L);

    // Lua error message parser
    lua_pushcfunction(L, traceback);

    // Load main lua script
    if (luaL_loadfile(L, "lua/main.lua")) {
        printf("%s: %s\n", argv[0], lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    // Evaluate main.lua to get program lifecycle function
    if (lua_pcall(L, 0, 1, -2)) {
        printf("%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Start program lifecycle
    // Load argv as initial args
    for (int i = 0; i < argc; i += 1) {
        lua_pushstring(L, argv[i]);
    }
    // Call lifecycle handler (which was returned by main.lua)
    if (lua_pcall(L, argc, 0, -(argc + 2))) {
        printf("%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}
