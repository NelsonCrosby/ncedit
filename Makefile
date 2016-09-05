OUT = ncedit
OBJ = ncedit
OBJ += lapi/lapi lapi/class lapi/screen

_CFLAGS = -std=c99 -MMD -Wall
_LDFLAGS =
_LDLIBS =

_PKG_LUA = lua5.2
ifeq ($(shell pkg-config --exists $(_PKG_LUA) &>/dev/null ; echo $$?),1)
 _PKG_LUA = lua
endif

PKG += ncurses $(_PKG_LUA)

_CFLAGS += `pkg-config --cflags $(PKG)`
_LDLIBS += `pkg-config --libs $(PKG)`

CFLAGS := $(_CFLAGS) $(CFLAGS)
LDFLAGS := $(_LDFLAGS) $(LDFLAGS)
LDLIBS := $(_LDLIBS) $(LDLIBS)

default: $(OUT)
clean:
	rm $(OUT) $(OBJ:=.o) $(OBJ:=.d)
.PHONY: default clean

$(OUT): $(OBJ:=.o)

-include $(OBJ:=.d)
