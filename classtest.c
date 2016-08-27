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

#include "classtest.h"
#include "lapi/class.h"


static int test_getvalue(lua_State *L)
{
    lua_newuserdata(L, sizeof(Test));
    return 1;
}


static int test_init(lua_State *L)
{
    Test *data = (Test*) lua_touserdata(L, 1);
    data->id = luaL_checkinteger(L, 2);
    return 0;
}


static int test_hello(lua_State *L)
{
    Test *data = (Test*) lua_touserdata(L, 1);
    printf("Hello!\t%d\n", data->id);
    return 0;
}


static luaL_Reg Test_proto[] = {
    {"[getvalue]", test_getvalue},
    {"init", test_init},
    {"hello", test_hello},
    {NULL, NULL}
};


int create_test_class(lua_State *L)
{
    create_class(L, 0);
    luaL_setfuncs(L, Test_proto, 0);
    lua_setglobal(L, "Test");
    return 0;
}


int run_test(lua_State *L)
{
    lua_getglobal(L, "Test2");
    lua_pushinteger(L, 3);
    create_inst(L, 1);
    call_method(L, -1, "goodbye", 0, 0);
    return 0;
}
