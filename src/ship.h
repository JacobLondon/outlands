#ifndef OUTLANDS_SHIP_H_
#define OUTLANDS_SHIP_H_

#include <stdbool.h>
#include "ship_tile.h"

typedef struct ship_tag ship;

ship *ship_new(char *name);
void ship_del(ship *self);
void ship_draw(ship *self);
void ship_update(ship *self);

/* load an rpg into an id matrix */
void ship_load_rpg(char *rpg, int *idmx, int width, int height);

/* Get a batch of unique walkable indices on the ship
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

#endif // OUTLANDS_SHIP_H_
