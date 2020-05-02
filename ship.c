#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ship_tile.h"
#include "ship.h"
#include "util.h"

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

ship *ship_new(void)
{

}

void ship_del(ship *self)
{

}

void ship_draw(ship *self)
{

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
