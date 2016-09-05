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

#include <lua.h>
#include <lauxlib.h>

#include "lapi.h"
#include "class.h"
#include "screen.h"


static luaL_Reg modules[] = {
    {"class", lapi_load_class},
    {"ncedit.screen", lapi_load_screen},
    {NULL, NULL}
};

// Function to set up lapi
int lapi_load(lua_State *L)
{
    // Load modules into package.preload
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    luaL_setfuncs(L, modules, 0);
    // Pop package and package.preload
    lua_pop(L, 2);

    // Load class module into globals
    lua_getglobal(L, "require");
    lua_pushstring(L, "class");
    lua_call(L, 1, 1);
    lua_setglobal(L, "new");

    return 0;
}
