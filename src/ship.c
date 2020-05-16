#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "texture_manager.h"
#include "scene_object.h"
#include "ship_tile.h"
#include "ship.h"
#include "util.h"
#include "globals.h"

/**
 * A ship is a grouping of cells where each cell is 40x40 pixels.
 * 
 * A ship has 'tiles' which are things that the people in the
 * ship can interact with. These tiles allow things to happen
 * such as heal the character interacting with it, teleport the
 * characters around it, etc... In addition, tiles may also
 * do nothing more than block the path for a character to walk
 * on, or it may be plain walkable and have no other feature.
 */

typedef struct ship_def_tag {
	char *name;
	char *rpg; /* what tiles go where in the ship */
	char *png; /* the image of the ship */
	int width; /* animation dims */
	int height; /* animation dims */
} ship_def;

typedef struct ship_tag {
	so *object; // the scene object of the ship texture
	ship_def *def;
	tile *tiles[GRIDS_TALL][GRIDS_WIDE];
	size_t tile_count;
} ship;

ship_def ship_definitions[] = {
	{
		.name = "Falcon",
		.rpg = "rpgs/test.rpg",
		.png = "assets/falcon-exterior.png",
		.width = 1,
		.height = 1
	},
	{
		NULL
	}
};


ship *ship_new(char *name)
{
	int i, j;
	int idmx[GRIDS_TALL][GRIDS_WIDE] = { 0 };
	ship_def *def = NULL;
	Texture2D *t;
	anim *a;
	ship *self = malloc(sizeof(ship));
	memset(self, 0, sizeof(ship));

	assert(self);
	assert(name);

	for (i = 0; ship_definitions[i].name != NULL; i++) {
		if (streq(name, ship_definitions[i].name)) {
			def = &ship_definitions[i];
			break;
		}
	}
	assert(("Ship definition not found", def != NULL));

	// load the rpg into the id matrix
	ship_load_rpg(def->rpg, (int *)idmx, GRIDS_WIDE, GRIDS_TALL);

	// convert the id matrix to actual tiles
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (idmx[i][j] > 0) {
				self->tiles[i][j] = ship_tile_new(idmx[i][j], j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT);
			}
		}
	}

	t = texman_load(def->png);
	a = anim_new(t, def->width, def->height);
	self->object = so_new_own(a);
	so_set_pos(self->object, 0, 0);
	self->def = def;
	self->tile_count = GRIDS_TALL * GRIDS_WIDE;

	return self;
}

void ship_del(ship *self)
{
	int i, j;
	assert(self);
	so_del(self->object);
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (self->tiles[i][j] != NULL) {
				ship_tile_del(self->tiles[i][j]);
			}
		}
	}
}

void ship_draw(ship *self)
{
	int i, j;
	assert(self);

	so_draw(self->object);
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (self->tiles[i][j] != NULL) {
				ship_tile_draw(self->tiles[i][j]);
			}
		}
	}
}

void ship_update(ship *self)
{
	int i, j;
	
	assert(self);
	so_update(self->object);
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (self->tiles[i][j] != NULL) {
				ship_tile_update(self->tiles[i][j]);
			}
		}
	}
}

void ship_load_rpg(char *rpg, int *idmx, int width, int height)
{
	char *text;
	char *p;
	int end;
	size_t size;
	int i, j, id;
	assert(rpg);
	assert(idmx);

	memset(idmx, 0, sizeof(int) * width * height);
	p = text = file_read(rpg, &size);

	while (p - text < size) {
		end = strcspn(p, "\n");
		if (sscanf(p, "%d%d%d", &i, &j, &id) != 3) {
			fprintf(stderr, "Failed reading line at %s\n", p);
			exit(1);
		}
		idmx[i * width + j] = id;

		// bypass the \n to get to next line so strcspn != 0 forever
		p += end + 1;
	}
	free(text);
}

size_t ship_find_walkable_batch(ship *self, int **xs, int **ys)
{
	int i, j;
	size_t count = 0;
	static int x[GRIDS_TALL * GRIDS_WIDE] = { 0 };
	static int y[GRIDS_TALL * GRIDS_WIDE] = { 0 };

	assert(self);
	assert(xs);
	assert(ys);

	memset(x, 0, sizeof(x));
	memset(y, 0, sizeof(y));
	*xs = x;
	*ys = y;

	// make sure there are enough walkable tiles
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (self->tiles[i][j] && ship_tile_get_walkable(self->tiles[i][j])) {
				x[count] = j;
				y[count] = i;
				count++;
			}
		}
	}
	return count;
}

bool ship_is_walkable(ship *self, int x, int y)
{
	assert(self);
	assert(0 <= x && x < GRIDS_WIDE);
	assert(0 <= y && y < GRIDS_TALL);
	if (self->tiles[y][x] == NULL) {
		return false;
	}
	return ship_tile_get_walkable(self->tiles[y][x]);
}

tile **ship_get_tiles(ship *self, size_t *size)
{
	assert(self);
	if (size != NULL) {
		*size = self->tile_count;
	}
	return self->tiles;
}
