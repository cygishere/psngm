bin_PROGRAMS = psngm

psngm_SOURCES = \
src/main.c

psngm_CFLAGS = $(shell pkg-config --cflags ncurses)
psngm_LDFLAGS = $(shell pkg-config --libs ncurses)

psngm_CFLAGS += -O2 -Wall -Wformat -Wformat=2 -Werror=format-security \
-Wconversion -Wimplicit-fallthrough -Wextra -g\
-fstack-clash-protection -fstack-protector-strong \
-Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now

psngm_LDFLAGS += -fsanitize=address,undefined,leak

psngm_CPPFLAGS = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3

