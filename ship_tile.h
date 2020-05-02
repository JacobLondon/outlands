#ifndef OUTLANDS_SHIP_MODULE_H_
#define OUTLANDS_SHIP_MODULE_H_

typedef struct tile_tag tile;

/* Give an asset path and get a tile back */
tile *ship_tile_new(char *png);
void ship_tile_del(tile *self);
void ship_tile_draw(tile *self);

char *ship_tile_get_png(int id);

/* get how many tile definitions there are */
int ship_tile_get_count(void);

#endif // OUTLANDS_SHIP_MODULE_H_
