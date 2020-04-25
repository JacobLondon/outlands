#ifndef OUTLANDS_OBJ_KEY_H_
#define OUTLANDS_OBJ_KEY_H_

/* A self contained animation
 * which goes once. Contains
 * its own state, useful for
 * things like one off animations
 * which go then stop, but can be
 * reused again later, or cloned
 * and multiple one-off animations
 * can go.
 * 
 * Note that anim key is not necessarily
 * part of the scene_manager ecosystem.
 */

#include <stdbool.h>
#include "scene_object.h"

typedef struct ko_tag ko;

ko *ko_new(void); /* kos are on the stack */
void ko_del(ko *self); /* dealloc itself */
ko *ko_copy(ko *self);
void ko_reset(ko *self);

/* So the whole idea about this is that
 * it has a similar interface to what
 * an scenes do, but you can give it
 * requirements or states it must reach
 * before it can go to the next one you
 * add to it.
 * 
 * For example:
 * I want this anim to start on the right
 * and move to the center. Once it reaches
 * the center, change anims to a new one,
 * and start circling. Circling is bound
 * to a global variable, which once set to
 * false moves the ko to the next state,
 * where it moves below the screen.
 * 
 * Make sure to use ko_set_key(self, true)
 * when done, or just change the global tied to it.
 * This will make the next state happen, if there is one.
 * If there are no more states, then it will ko_del
 * itself.
 * 
 * It's a good idea to make a bunch of prototypes,
 * and make copies for every one of these in the scene.
 * Don't even need to malloc, just copy right into any
 * ko buffer!
 */

typedef void (*ko_cb)(ko *self, so *object);

void ko_add_rate(ko *self, so *object, ko_cb cb_state, bool *key, float animation_rate);
#define ko_add(ObjectKeyP, SObjP, CbState, KeyP) \
	ko_add_rate(ObjectKeyP, SObjP, CbState, KeyP, 1.0f)

/* Updates an ko, returns true if done, false if still going
 * When it is finished, it will dealloc itself! check if it finished,
 * then reassign it as needed
 */
bool ko_update(ko *self);

/* look at the key of the current state
 * this handles whether something is
 * (void *)0 or (void *)1 or global
 */
bool ko_get_key(ko *self);

/* Don't use set key if you're doing
 * this with global variables... I
 * can't stop you though...
 * 
 * Well I could, but if you need it here it is
 */
void ko_set_key(ko *self, bool key);

float ko_get_frame(ko *self);

/* Set the position of all of its screen objects */
void ko_set_pos(ko *self, int x, int y);

#endif // OUTLANDS_OBJ_KEY_H_
