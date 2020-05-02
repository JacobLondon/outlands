#include <assert.h>
#include <stdbool.h>
#include <memory.h>
#include "ship_tile.h"
#include "entity.h"
#include "scene_object.h"
#include "texture_manager.h"
#include "util.h"

#define HEALTH_DEFAULT 100
#define HEALTH_INVINCIBLE 255

enum tile_id {
	TILE_NONE,
	TILE_FLOOR,
	TILE_DOOR,
	TILE_COUNT
};

typedef struct tile_def_tag {
	char *png;
	int width;
	int height;
	bool is_lightable;
	bool is_blockable;
	unsigned char health;
} tile_def;

typedef struct tile_tag {
	bool is_burning;
	bool is_evacuated; // of air
	bool is_poisoned;  // you'll need to evacuate this room before walking in it again!
	bool is_blocked;   // ie a door
	unsigned char health; // 255 remains unbreakable!!!!!!!!
	tile_def *def; // points to, does not own
	so *object; // owns
	entity *standing_entity; // NULL of no entity is standing
} tile;

static tile_def tile_definitions[] = {
	[TILE_NONE] = {
		NULL
	},
	[TILE_FLOOR] = { 
		.png = "assets/tiles/floor.png",
		.width = 1,
		.height = 1,
		.is_lightable = true,
		.is_blockable = false,
		.health = HEALTH_INVINCIBLE
	},
	[TILE_DOOR] = {
		.png = "assets/tiles/door.png",
		.width = 1,
		.height = 1,
		.is_lightable = true,
		.is_blockable = true,
		.health = HEALTH_DEFAULT
	},
	[TILE_COUNT] = {
		NULL
	}
};


tile *ship_tile_new(char *png)
{
	int i;
	Texture2D *t;
	anim *a;

	assert(png);
	tile *self = allocate(sizeof(tile));
	assert(self);
	memset(self, 0, sizeof(tile));

	for (i = 0; tile_definitions[i].png != NULL; i++) {
		if (streq(png, tile_definitions[i].png)) {
			self->health = tile_definitions[i].health;
			self->def = &tile_definitions[i];

			t = texman_load(png);
			a = anim_new(t, tile_definitions[i].width, tile_definitions[i].height);
			self->object = so_new_own(a);
			return self;
		}
	}

	assert(("Tile asset not defined", 0));
	return NULL;
}

void ship_tile_del(tile *self)
{
	assert(self);
	if (self->object) {
		so_del(self->object);
	}
	dealloc(self);
}

void ship_tile_draw(tile *self)
{
	assert(self);
	so_draw(self->object);
}

char *ship_tile_get_png(int id)
{
	if (id >= TILE_COUNT || id < 1) {
		return NULL;
	}

	return tile_definitions[id].png;
}

int ship_tile_get_count(void)
{
	return TILE_COUNT;
}