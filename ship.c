#include "ship_tile.h"
#include "ship.h"

/**
 * A ship is a grouping of cells where each cell is 32x32 pixels.
 * 
 * A ship has 'tiles' which are things that the people in the
 * ship can interact with. These tiles allow things to happen
 * such as heal the character interacting with it, teleport the
 * characters around it, etc... In addition, tiles may also
 * do nothing more than block the path for a character to walk
 * on, or it may be plain walkable and have no other feature.
 */

#define SHIP_SIZE_X_MAX 30
#define SHIP_SIZE_Y_MAX 25

typedef struct ship_tag {
	tile *tiles[SHIP_SIZE_Y_MAX * SHIP_SIZE_X_MAX]; // a 2D indexed array of tiles for the ship to take up

} ship;
