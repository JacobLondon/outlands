#include <assert.h>
#include <memory.h>
#include "ship_tile.h"
#include "entity.h"
#include "scene_object.h"
#include "texture_manager.h"
#include "util.h"

#define HEALTH_DEFAULT 100
#define HEALTH_INVINCIBLE 255

typedef struct tile_def_tag {
	char *png;
	int width;
	int height;
	bool is_lightable;
	bool is_blockable;
	unsigned char health;
} tile_def;

#define TILE_DEF_NONE() { NULL }
#define TILE_DEF_UNBREAKABLE(Png) { \
	.png = Png, \
	.width = 1, \
	.height = 1, \
	.is_lightable = false, \
	.is_blockable = true, \
	.health = HEALTH_INVINCIBLE \
}
#define TILE_DEF_LIGHTABLE(Png) { \
	.png = Png, \
	.width = 1, \
	.height = 1, \
	.is_lightable = false, \
	.is_blockable = false, \
	.health = HEALTH_INVINCIBLE \
}

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

enum tile_id {
	TILE_NONE,
	TILE_FLOOR,
	TILE_DOOR,
	TILE_BOX,
	TILE_CRATE,
	TILE_VENTBOX,
	TILE_FAN0,
	TILE_FAN1,
	TILE_CONTROLS0,
	TILE_CONTROLS1,
	TILE_WEAPONS0,
	TILE_WEAPONS1,
	TILE_WEAPONS2,
	TILE_WEAPONS3,
	TILE_COUNT
};

static tile_def tile_definitions[] = {
	[TILE_NONE]      = TILE_DEF_NONE(),
	[TILE_FLOOR]     = TILE_DEF_LIGHTABLE("assets/tiles/floor.png"),
	[TILE_DOOR]      = TILE_DEF_UNBREAKABLE("assets/tiles/door.png"),
	[TILE_BOX]       = TILE_DEF_UNBREAKABLE("assets/tiles/box.png"),
	[TILE_CRATE]     = TILE_DEF_UNBREAKABLE("assets/tiles/crate.png"),
	[TILE_VENTBOX]   = TILE_DEF_UNBREAKABLE("assets/tiles/ventbox.png"),
	[TILE_FAN0]      = TILE_DEF_UNBREAKABLE("assets/tiles/fan0.png"),
	[TILE_FAN1]      = TILE_DEF_UNBREAKABLE("assets/tiles/fan1.png"),
	[TILE_CONTROLS0] = TILE_DEF_UNBREAKABLE("assets/tiles/controls0.png"),
	[TILE_CONTROLS1] = TILE_DEF_UNBREAKABLE("assets/tiles/controls1.png"),
	[TILE_WEAPONS0]  = TILE_DEF_UNBREAKABLE("assets/tiles/weapons0.png"),
	[TILE_WEAPONS1]  = TILE_DEF_UNBREAKABLE("assets/tiles/weapons1.png"),
	[TILE_WEAPONS2]  = TILE_DEF_UNBREAKABLE("assets/tiles/weapons2.png"),
	[TILE_WEAPONS3]  = TILE_DEF_UNBREAKABLE("assets/tiles/weapons3.png"),
	[TILE_COUNT]     = TILE_DEF_NONE()
};


tile *ship_tile_new(int id, float x, float y)
{
	Texture2D *t;
	anim *a;

	assert(("Tile not defined", id > 0 && id < TILE_COUNT));
	tile *self = allocate(sizeof(tile));
	assert(self);
	memset(self, 0, sizeof(tile));

	self->health = tile_definitions[id].health;
	self->def = &tile_definitions[id];
	t = texman_load(tile_definitions[id].png);
	a = anim_new(t, tile_definitions[id].width, tile_definitions[id].height);
	self->object = so_new_own(a);
	so_set_pos(self->object, x, y);

	return self;
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

void ship_tile_update(tile *self)
{
	assert(self);
	so_update(self->object);
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

bool ship_tile_get_walkable(tile *self)
{
	assert(self);

	if (self->def == &tile_definitions[TILE_FLOOR] ||
	    self->def == &tile_definitions[TILE_DOOR])
	{
		if (self->is_blocked == false) {
			return true;
		}
	}
	return false;
}
