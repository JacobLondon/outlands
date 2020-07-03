#ifndef OUTLANDS_SO_H
#define OUTLANDS_SO_H

#include <stdbool.h>
#include "anim.h"

/**
 * Scene Object
 * Controls all scene objects and
 * apparent animations so we don't
 * have to do any of that.
 */
typedef struct so_tag so;
/* Move scene object if trigger == NULL or *trigger == true */
typedef void (*so_cb_movement)(so *self, float amt, bool *trigger);

so *so_new_owner(anim *animation, bool own_animation); /* define ownership of the animation */
#define so_new(AnimationP) so_new_owner(AnimationP, false) /* point to an animation */
#define so_new_own(AnimationP) so_new_owner(AnimationP, true) /* take ownership of the animation */
void so_del(so *self);
so *so_copy(so *self);
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

/**
 * A bunch of callbacks to pass into newmov and delmov
 * Also self is already asserted if these are used with
 * new/delmov
 */

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


#endif // OUTLANDS_SO_H
