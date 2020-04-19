#ifndef OUTLANDS_SO_H_
#define OUTLANDS_SO_H_

#include <stdbool.h>

/* Scene Object
 * Controls all scene objects and
 * apparent animations so we don't
 * have to do any of that. */
typedef struct so_tag so;
/* Move scene object if trigger == NULL or *trigger == true */
typedef void (*so_movement)(so *self, float amt, bool *trigger);

typedef struct Texture2D Texture2D;

so *so_new(Texture2D *png);
void so_del(so *self);
so *so_copy(so *other);
void so_newmov(so *self, so_movement movement, float amt, bool *trigger);
void so_delmov(so *self, so_movement movement);

/* set the position in screen space */
void so_set_pos(so *self, float x, float y);
void so_set_bobrate(so *self, float newrate);

void so_draw(so *self);
/* move by all movement callbacks */
void so_move(so *self);

/* self is already asserted... */

void so_move_left(so *self, float amt, bool *trigger);
void so_move_right(so *self, float amt, bool *trigger);
void so_move_up(so *self, float amt, bool *trigger);
void so_move_down(so *self, float amt, bool *trigger);
void so_move_bob_vrt(so *self, float amt, bool *trigger);
void so_move_bob_hrz(so *self, float amt, bool *trigger);
void so_move_loop_left(so *self, float amt, bool *trigger);
void so_move_loop_right(so *self, float amt, bool *trigger);
void so_move_loop_up(so *self, float amt, bool *trigger);
void so_move_loop_down(so *self, float amt, bool *trigger);


#endif // OUTLANDS_SO_H_
