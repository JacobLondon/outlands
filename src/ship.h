#ifndef OUTLANDS_SHIP_H
#define OUTLANDS_SHIP_H

#include <stddef.h>
#include "ship_tile.h"

typedef enum ship_type_tag {
	SHIP_PLAYER,
	SHIP_ENEMY,
	SHIP_COUNT
} ship_type;

typedef struct ship_tag ship;

ship *ship_new(char *name, ship_type type);
void ship_del(ship *self);
void ship_draw(ship *self);
void ship_update(ship *self);

/* load an rpg into an id matrix */
void ship_load_rpg(char *rpg, int *idmx, int width, int height);

/**
 * Get a batch of unique walkable indices on the ship
 * Return the maximum number of walkable spots on that ship,
 * the size of xs and ys.
 * xs and ys are pointers to func's static data indices
 * 
 * Calling the func will update the static arrays
 * NOT thread safe
 */
size_t ship_find_walkable_batch(ship *self, int **xs, int **ys);

bool ship_is_walkable(ship *self, int x, int y);

/* return pointer to tiles, assign size if not null */
tile **ship_get_tiles(ship *self, size_t *size);
int ship_get_width(ship *self);
int ship_get_height(ship *self);
int ship_get_tlx(ship *self);
int ship_get_tly(ship *self);

#endif // OUTLANDS_SHIP_H
