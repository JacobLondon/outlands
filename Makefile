CC=C:\raylib\tcc\tcc
LDFLAGS=C:\raylib\raylib\src\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
CFLAGS=-lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2

GAME_TARGET=lands.exe
MAKER_TARGET=maker.exe

FILES=\
	anim.c \
	animanager.c \
	context.c \
	globals.c \
	key_manager.c \
	key_object.c \
	scene_manager.c \
	scene_object.c \
	scene_object.c \
	scene.c \
	ship_tile.c \
	texture_manager.c \
	util.c \


GAME_FILES=\
	$(FILES) \
	main.c

MAKER_FILES=\
	$(FILES) \
	shipmaker.c

.PHONY: clean

all: $(GAME_TARGET)

maker: $(MAKER_TARGET)

$(GAME_TARGET): $(GAME_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(MAKER_TARGET): $(MAKER_FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	del $(GAME_TARGET) $(MAKER_TARGET)
