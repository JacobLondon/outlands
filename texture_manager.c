#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include "util.h"
#include "texture_manager.h"

#define TEXTURES_MAX 32

typedef struct texture_lookup_tag {
	char *png;
	Texture2D texture;
} texture_lookup;

static bool initialized = false;
static texture_lookup textures[TEXTURES_MAX] = { 0 };

void texman_init(void)
{
	assert(initialized == false);
	memset(textures, 0, TEXTURES_MAX);
	initialized = true;
}

void texman_cleanup(void)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (textures[i].png) {
			UnloadTexture(textures[i].texture);
		}
	}
	memset(textures, 0, TEXTURES_MAX);
	initialized = false;
}

void texman_load(char *png)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (textures[i].png == NULL) {
			textures[i].png = png;
			textures[i].texture = LoadTexture(png);
			break;
		}
		else if (streq(textures[i].png, png)) {
			break;
		}
	}
	assert(("Too many textures loaded", i < TEXTURES_MAX));
}

Texture2D *texman_get(char *png)
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
	assert(("Texture not found", i < TEXTURES_MAX));
}