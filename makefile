CC = g++
CFLAGS = -fPIC -Wall -Wextra -pedantic -std=c++23
LDFLAGS = -Lbin -Wl,-rpath=./bin
LDLIBS = -lengine

LIB_C_SRCS = $(wildcard src/lib/*.c)
LIB_CPP_SRCS = $(wildcard src/lib/*.cpp)
APP_C_SRCS = $(wildcard src/app/*.c)
APP_CPP_SRCS = $(wildcard src/app/*.cpp)

LIB_OBJS = $(LIB_C_SRCS:src/lib/%.c=obj/lib/%.o) $(LIB_CPP_SRCS:src/lib/%.cpp=obj/lib/%.o)
APP_OBJS = $(APP_C_SRCS:src/app/%.c=obj/app/%.o) $(APP_CPP_SRCS:src/app/%.cpp=obj/app/%.o)

LIBRARY = bin/libengine.so
PROGRAM = bin/app


.PHONY: all clean

all: directories $(LIBRARY) $(PROGRAM)

directories:
	@echo $(LIB_SRCS)
	@echo $(LIB_OBJS)
	mkdir -p bin $(dir $(LIB_OBJS)) $(dir $(APP_OBJS))

$(LIBRARY): $(LIB_OBJS)
	$(CC) -fPIC -shared -o $@ $^

$(PROGRAM): $(APP_OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) -r bin obj
