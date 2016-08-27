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

#include "lapi.h"
#include "class.h"


// Utility to create Lua modules that can be loaded by require
static void create_module(lua_State *L,
    const char *name, lua_CFunction loader);

// Loader for lapi class module
static int ldmod_class(lua_State *L);


// Function to set up lapi
int lapi_load(lua_State *L)
{
    // Create class module
    create_module(L, "class", ldmod_class);

    // Load class module into globals
    lua_getglobal(L, "require");
    lua_pushstring(L, "class");
    lua_call(L, 1, 1);
    lua_setglobal(L, "new");
    // Pop require function
    lua_pop(L, 1);

    return 0;
}


static void create_module(lua_State *L,
    const char *name, lua_CFunction loader)
{
    // Get package.preload
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    // Set function as preloader
    lua_pushcfunction(L, loader);
    lua_setfield(L, -2, name);
    // Pop package.preload then package
    lua_pop(L, 2);
}


// Wrapper for mt(new).__call to strip table inserted by __call
static int lapi_class_new_call(lua_State *L)
{
    lua_remove(L, 1);
    return lapi_class_new(L);
}

// Load lapi class
static int ldmod_class(lua_State *L)
{
    // Create table and set new.class
    lua_createtable(L, 0, 1);       // Create new
    lua_pushcfunction(L, lapi_class_sub);
    lua_setfield(L, -2, "class");   // Set class function in new
    // Set mt(new) and mt(new).__call
    lua_createtable(L, 0, 1);       // Create metatable for new
    lua_pushcfunction(L, lapi_class_new_call);
    lua_setfield(L, -2, "__call");  // Set call metamethod for new
    lua_setmetatable(L, -2);        // Set metatable for new
    // Return new
    return 1;
}
