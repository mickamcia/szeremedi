CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -Lbin -Wl,-rpath=./bin
LDLIBS = -lengine

LIB_SRCS = $(wildcard src/lib/*.c)
APP_SRCS = $(wildcard src/app/*.c)

LIB_OBJS = $(LIB_SRCS:src/%.c=obj/%.o)
APP_OBJS = $(APP_SRCS:src/%.c=obj/%.o)

LIBRARY = bin/libengine.so
PROGRAM = bin/app


.PHONY: all clean

all: directories $(LIBRARY) $(PROGRAM)

directories:
	mkdir -p bin $(dir $(LIB_OBJS)) $(dir $(APP_OBJS))

$(LIBRARY): $(LIB_OBJS)
	$(CC) -shared -o $@ $^

$(PROGRAM): $(APP_OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	$(RM) -r bin obj
