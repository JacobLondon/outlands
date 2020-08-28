#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "texture_man.h"
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
	int tly;    // top left y position
	int tlx;    // top left x position
	int width;  // farthest right point - farthest left point in grids
	int height; // lowest point - highest point in grids
} ship;

ship_def ship_definitions[] = {
	{
		.name = "Falcon",
		.rpg = "rpgs/falcon.rpg",
		.png = ASSET_DIRECTORY "/ext_falcon.png",
		.width = 1,
		.height = 1
	},
	{
		.name = "Sword",
		.rpg = "rpgs/sword.rpg",
		.png = ASSET_DIRECTORY "/ext_sword.png",
		.width = 1,
		.height = 1
	},
	{
		NULL
	}
};


ship *ship_new(char *name, ship_type type)
{
	int i, j, k;
	// ship dimensions
	int hi_i = 0;
	int hi_j = 0;
	int lo_i = -1;
	int lo_j = -1;
	int height, width;
	// id matrix for rpg loading
	int idmx[GRIDS_TALL][GRIDS_WIDE] = { 0 };
	ship_def *def = NULL;
	// texture for background ship
	Texture2D *t;
	anim *a;
	ship *self = malloc(sizeof(ship));
	assert(self);
	assert(name);
	memset(self, 0, sizeof(ship));

	for (i = 0; ship_definitions[i].name != NULL; i++) {
		if (streq(name, ship_definitions[i].name)) {
			def = &ship_definitions[i];
			break;
		}
	}
	msg_assert(def != NULL, "Ship definition not found: %s", name);

	// load the rpg into the id matrix
	ship_load_rpg(def->rpg, (int *)idmx, GRIDS_WIDE, GRIDS_TALL);

	// save the extreme positions
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (idmx[i][j] > 0) {
				// save first found, most top left point
				if (lo_i == -1) {
					lo_i = i;
				}
				// save most right point
				if (lo_j == -1 || j < lo_j) {
					lo_j = j;
				}
				// keep saving the greatest points
				if (i >= hi_i) {
					hi_i = i;
				}
				if (j >= hi_j) {
					hi_j = j;
				}
			}
		}
	}
	width = hi_j - lo_j;
	height = hi_i - lo_i;

	// move ship into position, bottom left
	if (type == SHIP_PLAYER) {
		// move down
		if (GRIDS_TALL - (lo_i + height) > 1) {
			for (k = GRIDS_TALL - 2, i = lo_i + height; i >= 0; i--, k--) {
				for (j = 0; j < GRIDS_WIDE; j++) {
					idmx[k][j] = idmx[i][j];
					idmx[i][j] = 0;
				}
			}
		}
		// just assume that it's at or above the needed height...

		// move left
		if (lo_j > 1) {
			for (i = lo_i; i < GRIDS_TALL; i++) {
				for (k = 1, j = lo_j; j < GRIDS_WIDE; j++, k++) {
					idmx[i][k] = idmx[i][j];
					idmx[i][j] = 0;
				}
			}
		}
	}
	// ship ship into position, top right
	// TODO: This is SUPER broken for the "Sword", let's find out why
	else if (type == SHIP_ENEMY) {
		// move up
		if (lo_i > 1) {
			for (k = 1, i = lo_i; i < GRIDS_TALL; i++, k++) {
				for (j = lo_j; j < GRIDS_WIDE; j++) {
					idmx[k][j] = idmx[i][j];
					idmx[i][j] = 0;
				}
			}
		}

		// move right
		if (lo_j + width < GRIDS_WIDE) {
			for (i = lo_i; i < GRIDS_TALL; i++) {
				for (k = GRIDS_WIDE - 4, j = lo_j + width; j >= 0; j--, k--) {
					idmx[i][k] = idmx[i][j];
					idmx[i][j] = 0;
				}
			}
		}
	}
	else {
		msg_assert(0, "Bad path");
	}

	// convert the id matrix to actual tiles
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (idmx[i][j] > 0) {
				self->tiles[i][j] = ship_tile_new(idmx[i][j], j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT);
			}
		}
	}

	t = texture_man_load(def->png);
	a = anim_new(t, def->width, def->height);
	self->object = so_new_own(a);
	if (type == SHIP_PLAYER) {
		// bottom left
		so_set_pos(self->object, 0, GetScreenHeight() - anim_get_height(so_get_anim(self->object)));
	}
	else {
		// top right
		so_set_pos(self->object, GetScreenWidth() - anim_get_width(so_get_anim(self->object)), 0);
	}
	self->def = def;
	self->tile_count = GRIDS_TALL * GRIDS_WIDE;
	self->width = width;
	self->height = height;
	self->tlx = lo_j;
	self->tly = lo_i;

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
	return (tile **)self->tiles;
}

int ship_get_width(ship *self)
{
	assert(self);
	return self->width;
}

int ship_get_height(ship *self)
{
	assert(self);
	return self->height;
}

int ship_get_tlx(ship *self)
{
	assert(self);
	return self->tlx;
}

int ship_get_tly(ship *self)
{
	assert(self);
	return self->tly;
}
