OUT = ncedit
OBJ = ncedit classtest
OBJ += lapi/lapi lapi/class

_CFLAGS = -std=c99 -MMD
_LDFLAGS =
_LDLIBS =
PKG += ncurses lua5.2

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
