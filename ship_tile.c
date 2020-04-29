#include <assert.h>
#include <stdbool.h>
#include "ship_tile.h"
#include "entity.h"
#include "scene_object.h"
#include "texture_manager.h"
#include "util.h"

#define HEALTH_DEFAULT 100
#define HEALTH_INVINCIBLE 255

enum tile_id_tag {
	TILE_FLOOR,
	TILE_WALL,
	TILE_DOOR,
	TILE_SPECIAL, // if the tile does something, run its callback
	TILE_COUNT
};

typedef struct tile_attr_tag {
	bool is_walkable;  // can walk there
	bool is_lightable; // light-on-fireable
	bool is_blockable  // something that is blocking can be broken to be unblocking
} tile_attr;

typedef struct tile_def_tag {
	char *png;
	int width;
	int height;
	unsigned char health;
	tile_attr *attr;
} tile_def;

typedef struct tile_tag {
	bool is_burning;
	bool is_evacuated; // of air
	bool is_poisoned;  // you'll need to evacuate this room before walking in it again!
	unsigned char health; // 255 remains unbreakable!!!!!!!!
	tile_attr *attr;
	so *object;
	entity *standing_entity; // NULL of no entity is standing
} tile;

typedef tile *(*tile_cb_new)(tile_def *def);

static tile_attr tile_attributes[] = {
	[TILE_FLOOR]   = { true,  true,  false },
	[TILE_WALL]    = { false, false, false },
	[TILE_DOOR]    = { true,  false, true  },
	[TILE_SPECIAL] = { true,  true,  false },
};


static tile_def tile_definitions[] = {
	{ "assets/floor.png", 1, 1, HEALTH_INVINCIBLE, &tile_attributes[TILE_FLOOR] },
	{ "assests/space.png", 1, 1, HEALTH_INVINCIBLE, &tile_attributes[TILE_WALL] },
	{ "assests/door.png",  1, 1, HEALTH_DEFAULT, &tile_attributes[TILE_DOOR] },
	{ NULL, 0, 0, 0, NULL }
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
			self->attr = tile_definitions[i].attr;

			t = texman_load(png);
			a = anim_new(t, tile_definitions[i].width, tile_definitions[i].height);
			self->object = so_new_own(a);
			return self;
		}
	}

	assert(("Tile asset not defined", 0));
	return NULL;
}
