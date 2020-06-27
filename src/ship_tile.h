#ifndef OUTLANDS_SHIP_MODULE_H
#define OUTLANDS_SHIP_MODULE_H

#include <stdbool.h>

typedef struct tile_tag tile;

/* Give a tile id and get a tile back */
tile *ship_tile_new(int id, float x, float y);
void ship_tile_del(tile *self);
void ship_tile_draw(tile *self);
void ship_tile_update(tile *self);

char *ship_tile_get_png(int id);

/* get how many tile definitions there are */
int ship_tile_get_count(void);

bool ship_tile_get_walkable(tile *self);

#endif // OUTLANDS_SHIP_MODULE_H
