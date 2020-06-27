ifeq ($(OS),Windows_NT)
	CC=C:\raylib\tcc\tcc
	LDFLAGS = C:\raylib\raylib\src\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS  = -lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2 -Isrc/
	TARGET_OUTLANDS = outlands.exe
	TARGET_EDITOR   = editor.exe
	TARGET_TEST     = test.exe
	REMOVE          = del
else
	CC=gcc
	LDFLAGS = -O2 -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS  = -lraylib -lm -O2 -Isrc/
	TARGET_OUTLANDS = outlands
	TARGET_EDITOR   = editor
	TARGET_TEST     = test
	REMOVE          = rm -f
endif

SOURCES = $(wildcard src/*.c)
SOURCES_LANDS = $(SOURCES) tools/lands.c
SOURCES_MAKER = $(SOURCES) tools/editor.c
SOURCES_TEST  = $(SOURCES) tools/test.c

.PHONY: clean rule_editor rule_outlands check release


all: rule_outlands

release: CFLAGS+=-DNDEBUG
release: $(TARGET_OUTLANDS)

rule_outlands: $(TARGET_OUTLANDS)

rule_editor: $(TARGET_EDITOR)

check: $(TARGET_TEST)
	./$(TARGET_TEST)


$(TARGET_OUTLANDS): $(SOURCES_LANDS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(DEBUG)

$(TARGET_EDITOR): $(SOURCES_MAKER)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(DEBUG)

$(TARGET_TEST): $(SOURCES_TEST)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(DEBUG)

clean:
	$(REMOVE) $(TARGET_OUTLANDS) $(TARGET_EDITOR) $(TARGET_TEST)
