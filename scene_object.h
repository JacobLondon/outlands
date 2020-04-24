#ifndef OUTLANDS_SO_H_
#define OUTLANDS_SO_H_

#include <stdbool.h>
#include "anim.h"

/* Scene Object
 * Controls all scene objects and
 * apparent animations so we don't
 * have to do any of that. */
typedef struct so_tag so;
/* Move scene object if trigger == NULL or *trigger == true */
typedef void (*so_cb_movement)(so *self, float amt, bool *trigger);

so *so_new(anim *animation);
void so_del(so *self);
so *so_copy(so *other);
void so_newmov(so *self, so_cb_movement movement, float amt, bool *trigger);
void so_delmov(so *self, so_cb_movement movement);

/* set the position in screen space */
void so_set_pos(so *self, float x, float y);
Vector2 so_get_pos(so *self);
anim *so_get_anim(so *self);
void so_set_bobrate(so *self, float newrate);

void so_draw(so *self);
/* move by all movement callbacks */
void so_update(so *self);

/* self is already asserted... */

void so_cb_trk_vrt(so *self, float amt, bool *trigger); /* vertical mouse track */
void so_cb_trk_hrz(so *self, float amt, bool *trigger); /* horizontal mouse track */
void so_cb_left(so *self, float amt, bool *trigger);
void so_cb_right(so *self, float amt, bool *trigger);
void so_cb_up(so *self, float amt, bool *trigger);
void so_cb_down(so *self, float amt, bool *trigger);
void so_cb_bob_vrt(so *self, float amt, bool *trigger);
void so_cb_bob_hrz(so *self, float amt, bool *trigger);
void so_cb_loop_left(so *self, float amt, bool *trigger);
void so_cb_loop_right(so *self, float amt, bool *trigger);
void so_cb_loop_up(so *self, float amt, bool *trigger);
void so_cb_loop_down(so *self, float amt, bool *trigger);


#endif // OUTLANDS_SO_H_
