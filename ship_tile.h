#ifndef OUTLANDS_SHIP_MODULE_H_
#define OUTLANDS_SHIP_MODULE_H_

typedef struct tile_attr_tag tile_attr;
typedef struct tile_tag tile;

/* Give an asset path and get a tile back */
tile *ship_tile_new(char *png);

void ship_tile_draw(tile *self);

#endif // OUTLANDS_SHIP_MODULE_H_
