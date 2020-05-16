#include <assert.h>
#include <memory.h>
#include "globals.h"
#include "ship_tile.h"
#include "ship_manager.h"

static bool initialized = false;

// a 2D indexed array of tiles for the ships to take up
static tile *tiles[GRIDS_TALL][GRIDS_WIDE];
static ship *ships[SHIP_COUNT] = { 0 };

void ship_manager_init(void)
{
	initialized = true;
	memset(tiles, 0, sizeof(tiles));
	memset(ships, 0, sizeof(ships));
}

void ship_manager_cleanup(void)
{
	int i;
	assert(initialized);
	for (i = 0; i < SHIP_COUNT; i++) {
		if (ships[i] != NULL) {
			ship_del(ships[i]);
		}
	}
	initialized = false;
}

// TODO: Move ship to upper/lower section depending on player/enemy
ship *ship_manager_load(char *name, ship_type type)
{
	tile **tmp = tiles;
	tile **ship_tiles = NULL;
	size_t count = 0;
	int i;
	assert(name);

	ships[type] = ship_new(name);
	ship_tiles = ship_get_tiles(ships[type], &count);
	for (i = 0; i < count; i++) {
		if (ship_tiles[i] != NULL) {
			tmp[i] = ship_tiles[i];
		}
	}
	return ships[type];
}

void ship_manager_draw(void)
{
	int i;
	assert(initialized);
	for (i = 0; i < SHIP_COUNT; i++) {
		if (ships[i] != NULL) {
			ship_draw(ships[i]);
		}
	}
}

void ship_manager_update(void)
{
	int i;
	assert(initialized);
	for (i = 0; i < SHIP_COUNT; i++) {
		if (ships[i] != NULL) {
			ship_update(ships[i]);
		}
	}
}

bool ship_manager_is_walkable(int x, int y)
{
	assert(0 <= x && x < GRIDS_WIDE);
	assert(0 <= y && y < GRIDS_TALL);
	if (tiles[y][x] == NULL) {
		return false;
	}
	return ship_tile_get_walkable(tiles[y][x]);
}
