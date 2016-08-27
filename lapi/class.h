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

#pragma once


/** new(class, ...)
 * Creates a new instance of a class,
 *  and calls the constructor using rest of arguments
 */
int lapi_class_new(lua_State *L);

/** new.class(...)
 * Creates a new class, using arguments as parents.
 * All parents are treated as mixins, and are applied right-to-left
 *  (so left-most parents overwrite right-most parents).
 */
int lapi_class_sub(lua_State *L);


// Wrappers for use in C code

/** [-nparents, +1, e]
 * Pops nparents elements off the stack,
 *  and uses them to create a new prototype.
 * The recommended way of populating this
 *  prototype is using luaL_setfuncs().
 * Note that if proto['[getvalue]'] is a
 *  function, it will be used to get the
 *  base value.
 */
void create_class(lua_State *L, int nparents);

/** [-cargs+1, +1, e]
 * Uses prototype at stack index -(cargs+1)
 *  to create a new instance of this class.
 * The top cargs values on the stack are
 *  used as constructor arguments.
 * Note that if proto['[getvalue]'] is a
 *  function, it will be used to get the
 *  base value.
 */
void create_inst(lua_State *L, int cargs);

/** [-nargs, +nresults, e]
 * Calls a method mname on the object at stack index idx,
 *  using using nargs from the stack and resulting in
 *  nresults.
 */
void call_method(lua_State *L,
    int idx, const char *mname, int nargs, int nresults);
