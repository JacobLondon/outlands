#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "texture_man.h"

#define TEXTURES_MAX 32

typedef struct texture_lookup_tag {
	char *png;
	Texture2D texture;
} texture_lookup;

static bool initialized = false;
static texture_lookup textures[TEXTURES_MAX] = { 0 };

void texture_man_init(void)
{
	assert(initialized == false);
	memset(textures, 0, sizeof(textures));
	initialized = true;
}

void texture_man_cleanup(void)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (textures[i].png) {
			UnloadTexture(textures[i].texture);
		}
	}
	memset(textures, 0, sizeof(textures));
	initialized = false;
}

Texture2D *texture_man_load(char *png)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (textures[i].png == NULL) {
			textures[i].png = png;
			textures[i].texture = LoadTexture(png);
			return &textures[i].texture;
		}
		else if (streq(textures[i].png, png)) {
			return &textures[i].texture;
		}
	}
	msg_assert("Too many textures loaded", i < TEXTURES_MAX);
	return NULL;
}

Texture2D *texture_man_get(char *png)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (textures[i].png == NULL) {
			continue;
		}
		else if (streq(textures[i].png, png)) {
			return &textures[i].texture;
		}
	}
	msg_assert("Texture not found", i < TEXTURES_MAX);
	return NULL;
}

void texture_man_test_draw(Texture2D *texture, int x, int y)
{
	assert(texture);
	DrawTexture(*texture, x, y, WHITE);
}
