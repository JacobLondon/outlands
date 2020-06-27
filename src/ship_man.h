#ifndef OUTLANDS_SHIP_MAN_H
#define OUTLANDS_SHIP_MAN_H

#include <stdbool.h>
#include "ship.h"

typedef enum ship_type_tag {
	SHIP_PLAYER,
	SHIP_ENEMY,
	SHIP_COUNT
} ship_type;

void ship_man_init(void);
void ship_man_cleanup(void);
void ship_man_draw(void);
void ship_man_update(void);
ship *ship_man_load(char *name, ship_type type);

bool ship_man_is_walkable(int x, int y);
ship *ship_man_get(ship_type type);

#endif // OUTLANDS_SHIP_MAN_H
