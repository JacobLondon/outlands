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

typedef struct object_key_tag object_key;

object_key *object_key_new(void); /* object_keys are on the stack */
void object_key_del(object_key *self); /* ref to pointer to nullilfy itself */
object_key *object_key_copy(object_key *self);

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
 * false moves the object_key to the next state,
 * where it moves below the screen.
 * 
 * Make sure to use object_key_set_key(self, true)
 * when done, or just change the global tied to it.
 * This will make the next state happen, if there is one.
 * If there are no more states, then it will object_key_del
 * itself.
 * 
 * It's a good idea to make a bunch of prototypes,
 * and make copies for every one of these in the scene.
 * Don't even need to malloc, just copy right into any
 * object_key buffer!
 */

typedef void (*object_key_cb)(object_key *self, so *object);

void object_key_add_rate(object_key *self, so *object, object_key_cb cb_state, bool *key, float animation_rate);
#define object_key_add(ObjectKeyP, SObjP, CbState, KeyP) \
	object_key_add_rate(ObjectKeyP, SObjP, CbState, KeyP, 1.0f)

/* Updates an object_key, returns true if done, false if still going
 * When it is finished, it will dealloc itself! check if it finished,
 * then reassign it as needed
 */
bool object_key_update(object_key *self);

/* look at the key of the current state
 * this handles whether something is
 * (void *)0 or (void *)1 or global
 */
bool object_key_get_key(object_key *self);

/* Don't use set key if you're doing
 * this with global variables... I
 * can't stop you though...
 * 
 * Well I could, but if you need it here it is
 */
void object_key_set_key(object_key *self, bool key);

float object_key_get_frame(object_key *self);

#endif // OUTLANDS_OBJ_KEY_H_
