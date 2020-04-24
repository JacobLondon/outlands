#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include <raylib.h>
#include "util.h"
#include "obj_key.h"

#define STATES_MAX 6 /* max number of states for an object_key to manage */

typedef struct object_key_tag {
	so *objects[STATES_MAX]; /* just point, don't own */
	object_key_cb state_cbs[STATES_MAX];

	/* So this is gonna be a weird one.
	 * These keys are the state variables for
	 * controlling objects. Now when
	 * object_key add state is called, if the
	 * key variable passed is NULL,
	 * then the object_key will automatically control
	 * that bool. If a key is passed, then the object_key
	 * will have to wait on what is passed before
	 * progressing to the next state.
	 * 
	 * If I own, then NULL (void *)0 means false
	 * and (void *)1 means true
	 */
	bool *keys[STATES_MAX];  /* Own some, not all. If false, they state goes,
	                            if true, the state goes to the next*/
	bool mykeys[STATES_MAX]; /* true = I own, false = global bool */
	float rates[STATES_MAX]; /* Number of frames to animate per second per state */
	float oof; /* Time of the current animation. If this increases past the current
	              rate, then use the next frame of the animation */
	int frame; /* the frame number the animation is in */
	int state;
} object_key;

object_key *object_key_new(void)
{
	object_key *self = malloc(sizeof(object_key));
	assert(self);
	memset(self, 0, sizeof(object_key));
	return self;
}

void object_key_del(object_key *self)
{
	assert(self);
	memset(self, 0, sizeof(object_key));
	free(self);
}

object_key *object_key_copy(object_key *self)
{
	int i;
	assert(self);
	object_key *other = malloc(sizeof(object_key));
	assert(other);
	memcpy(other, self, sizeof(object_key));

	// visual defaults, internals copied above
	other->frame = 0;
	other->state = 0;
	for (i = 0; i < STATES_MAX; i++) {
		if (other->objects[i]) {
			anim_reset(so_get_anim(other->objects[i]));
		}
	}
	return other;
}

void object_key_add_rate(object_key *self, so *object, object_key_cb cb_state, bool *key, float animation_rate)
{
	int i;

	assert(self);
	assert(object);
	assert(cb_state);
	
	for (i = 0; i < STATES_MAX; i++) {
		if (self->objects[i] == NULL) {
			self->objects[i] = object;
			self->state_cbs[i] = cb_state;
			if (key) {
				self->keys[i] = key;
			}
			else {
				self->mykeys[i] = true;
			}
			self->rates[i] = animation_rate;
			break;
		}
	}
	assert(("Too many object keys", i < STATES_MAX));
}

bool object_key_update(object_key *self)
{
	assert(self);
	object_key_cb cb = self->state_cbs[self->state];
	cb(self, self->objects[self->state]);

	// done with state, go to next one
	if (object_key_get_key(self)) {
		self->oof = 0.0f;
		self->state++;

		if (self->state > STATES_MAX || self->objects[self->state] == NULL) {
			object_key_del(self);
			return true;
		}
	}

	so_draw(self->objects[self->state]);

	// update object if necessary
	so_update(self->objects[self->state]);
	if (self->oof > 1.0f / self->rates[self->state]) {
		anim_update(so_get_anim(self->objects[self->state]));
		self->oof = 0.0f;
		self->frame++;
	}
	self->oof += GetFrameTime();
	return false;
}

bool object_key_get_key(object_key *self)
{
	assert(self);

	// I own
	if (self->mykeys[self->state] == true) {
		return self->keys[self->state] != NULL;
	}
	// I don't own, global
	else {
		assert(self->keys[self->state]); // careful
		return *self->keys[self->state];
	}
}

void object_key_set_key(object_key *self, bool key)
{
	assert(self);

	// I own
	if (self->mykeys[self->state] == true) {
		self->keys[self->state] = (void *)key;
	}
	// I don't own, global
	else {
		assert(self->keys[self->state]); // careful
		*self->keys[self->state] = key;
	}
}

float object_key_get_frame(object_key *self)
{
	assert(self);
	return self->frame;
}
