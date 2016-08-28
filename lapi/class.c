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

#include "class.h"


int lapi_class_new(lua_State *L)
{
    // Get number of args for init
    int nargs = lua_gettop(L) - 1;

    // Check if prototype generates its own value
    lua_getfield(L, 1, "[getvalue]");
    if (lua_isfunction(L, -1)) {
        // Call function to get custom value
        lua_call(L, 0, 1);
    } else {
        // Remove non-function value from getfield
        lua_pop(L, 1);
        // Create instance table
        lua_newtable(L);
    }

    // Use first argument as prototype
    lua_pushvalue(L, 1);    // Copy arg to top
    lua_remove(L, 1);       // Don't need arg at 0
    lua_setmetatable(L, -2);

    // Check for init function
    lua_getfield(L, -1, "init");
    if (lua_isfunction(L, -1)) {
        // Call init function:
        lua_insert(L, 1);   // Move function into place
        lua_insert(L, 2);   // Move instance into place
        // Duplicate instance and move to bottom
        lua_pushvalue(L, 2);
        lua_insert(L, 1);
        // Perform call
        lua_call(L, nargs + 1, 1);
        // Ensure top is the right value
        if (!lua_toboolean(L, -1)) {
            // Top is not to be used as instance
            lua_pop(L, 1);
        } // Else top is the new instance
        // init done
    } else {
        // Remove non-function init from top
        lua_pop(L, 1);
    }

    // Top is new instance, return it
    return 1;
}


int lapi_class_sub(lua_State *L)
{
    // Create prototype table
    lua_newtable(L);
    lua_insert(L, 1);
    // Set __index of prototype to itself
    lua_pushvalue(L, 1);
    lua_setfield(L, -2, "__index");

    // Mixin parents
    while (lua_gettop(L) > 1) /* Will be false when no more parents left */ {
        // Traverse table pairs
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Duplicate key for use in next iteration
            lua_pushvalue(L, -2);
            lua_insert(L, -2);  // Stack now looks like ..., k, k, v
            // Set value in prototype
            lua_settable(L, 1);
            // Stack is now topped with key for next iter
        }
        // Remove parent so next can be used
        lua_pop(L, 1);
    }

    // Return prototype
    lua_pushvalue(L, 1);
    return 1;
}


void create_class(lua_State *L, int nparents)
{
    lua_pushcfunction(L, lapi_class_sub);
    lua_insert(L, -(nparents + 1));
    lua_call(L, nparents, 1);
}


void create_inst(lua_State *L, int cargs)
{
    lua_pushcfunction(L, lapi_class_new);
    lua_insert(L, -(cargs + 2));
    lua_call(L, cargs + 1, 1);
}


void call_method(lua_State *L,
    int idx, const char *mname, int nargs, int nresults)
{
    // Get duplicate reference to value for use as argument
    lua_pushvalue(L, idx);
    lua_insert(L, -(nargs + 1));
    // Get method to call
    lua_getfield(L, -(nargs + 1), mname);
    lua_insert(L, -(nargs + 2));
    // Call method
    lua_call(L, nargs + 1, nresults);
}
