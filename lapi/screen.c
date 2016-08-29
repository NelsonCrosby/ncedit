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

#include <signal.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <ncurses.h>

#include "../lapi/class.h"
#include "screen.h"


static int Screen_getvalue(lua_State *L)
{
    lua_newuserdata(L, sizeof(Screen));
    return 1;
}


// Used to ensure we only ever have one Screen open at a time
static bool initialized = false;

static int Screen_init(lua_State *L)
{

    if (initialized) {
        // Avoid creating multiple ncurses screens
        //  (what would that even do?)
        return luaL_error(L, "only allowed one Screen open at a time");
    }

    initialized = true;
    initscr();
    nodelay(stdscr, true);
    // TODO: more init based on passed settings.

    return 0;
}


static int Screen_close(lua_State *L)
{
    endwin();
    initialized = false;
    return 0;
}


static int Screen_hello(lua_State *L)
{
    int ctx;
    switch (lua_getctx(L, &ctx)) {

    case LUA_OK:
        printw(luaL_checkstring(L, 2));
        refresh();
    case LUA_YIELD:
        if (getch() == ERR) {
            return lua_yieldk(L, 0, ctx, Screen_hello);
        }
        return lua_yield(L, 0);

    default:    // Shouldn't ever happen
        raise(SIGABRT);
        return 0;
    }
}


static int Screen_gc(lua_State *L)
{
    Screen_close(L);
    return 0;
}


static luaL_Reg methods[] = {
    {"[getvalue]", Screen_getvalue},
    {"init", Screen_init},
    {"close", Screen_close},
    {"hello", Screen_hello},
    {"__gc", Screen_gc},
    {NULL, NULL}
};

int lapi_load_screen(lua_State *L)
{
    create_class(L, 0);
    luaL_setfuncs(L, methods, 0);
    return 1;
}
