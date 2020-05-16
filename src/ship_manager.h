#ifndef OUTLANDS_SHIP_MANAGER_H_
#define OUTLANDS_SHIP_MANAGER_H_

#include <stdbool.h>
#include "ship.h"

typedef enum ship_type_tag {
	SHIP_PLAYER,
	SHIP_ENEMY,
	SHIP_COUNT
} ship_type;

void ship_manager_init(void);
void ship_manager_cleanup(void);
void ship_manager_draw(void);
void ship_manager_update(void);
ship *ship_manager_load(char *name, ship_type type);

bool ship_manager_is_walkable(int x, int y);

#endif // OUTLANDS_SHIP_MANAGER_H_
