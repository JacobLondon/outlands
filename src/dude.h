#ifndef OUTLANDS_DUDE_H_
#define OUTLANDS_DUDE_H_

#include "ship.h"

typedef struct dude_tag dude;

/* All dudes are stack dudes.
 * You can't get them, and they
 * exist only in dude.c
 */

void dude_load(size_t numberof, char *name, ship *other);
void dude_unload(void);

// do all of the jobs for the dudes
void dude_update(void);
// draw all dudes
void dude_draw(void);

// this is a big problem, we need to get all available tiles
// regardless of which ship owns them
void dude_select_update(ship *other);
void dude_select_draw(void);

void dude_job_assign(unsigned char id, int x, int y);

#endif // OUTLANDS_DUDE_H_
