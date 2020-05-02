#ifndef OUTLANDS_SHIP_H_
#define OUTLANDS_SHIP_H_

typedef struct ship_tag ship;

ship *ship_new(void);
void ship_del(ship *self);
void ship_draw(ship *self);

/* load an rpg into an id matrix */
void ship_load_rpg(char *rpg, int *idmx, int width, int height);

#endif // OUTLANDS_SHIP_H_
