OUT = ncedit
OBJ = ncedit

_CFLAGS += -std=c99
_LDFLAGS +=

default: ncedit
.PHONY: default

$(OBJ:%=%.o): %.o: %.c
	cc $(_CFLAGS) `pkg-config --cflags ncurses lua5.2` -MMD -c $< -o $@

$(OUT): $(OBJ:%=%.o)
	cc $(_LDFLAGS) $^ -o $@ `pkg-config --libs ncurses lua5.2`
