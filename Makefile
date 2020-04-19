CC=C:\raylib\tcc\tcc
LDFLAGS=C:\raylib\raylib\src\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
CFLAGS=-lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2

TARGET=lands.exe

FILES=\
	imger.c \
	scene_manager.c \
	scene.c \
	scene_object.c \
	util.c \
	main.c

.PHONY: clean

all: $(TARGET)

$(TARGET): $(FILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	del $(TARGET)
