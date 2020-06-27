ifeq ($(OS),Windows_NT)
	CC=C:\raylib\tcc\tcc
	LDFLAGS=C:\raylib\raylib\src\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS=-lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2 -Isrc/
	TARGET_LANDS = outlands.exe
	TARGET_MAKER = maker.exe
	TARGET_TEST  = test.exe
	REMOVE = del
else
	CC=gcc
	LDFLAGS=-Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS=-lraylib -lm -O2 -Isrc/
	TARGET_LANDS = outlands
	TARGET_MAKER = maker
	TARGET_TEST  = test
	REMOVE = rm
endif

SOURCES = $(wildcard src/*.c)
SOURCES_LANDS = $(SOURCES) tools/lands.c
SOURCES_MAKER = $(SOURCES) tools/maker.c
SOURCES_TEST  = $(SOURCES) tools/test.c

.PHONY: clean test maker lands

all: lands
maker: $(TARGET_MAKER)
lands: $(TARGET_LANDS)
test: $(TARGET_TEST)

$(TARGET_LANDS): $(SOURCES_LANDS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(TARGET_MAKER): $(SOURCES_MAKER)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(TARGET_TEST): $(SOURCES_TEST)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	$(REMOVE) $(TARGET_LANDS) $(TARGET_MAKER) $(TARGET_TEST)
