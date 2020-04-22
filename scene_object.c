#include <assert.h>
#include <math.h>
#include <memory.h>
#include <raylib.h>
#include "scene_object.h"
#include "util.h"

#define MOVEMENTS_MAX 4

typedef struct so_tag {
	Vector2 pos;
	float bobrate;
	float bobdelta;
	bool is_offscreen;
	struct {
		so_movement movement;
		bool *trigger;
		float amt;
	} movements[MOVEMENTS_MAX];
	anim *animation;
} so;


so *so_new(anim *animation)
{
	assert(animation);
	so *self = allocate(sizeof(so));
	assert(self);
	self->pos = (Vector2){GetScreenWidth() / 2, 0};
	memset(self->movements, 0, sizeof(self->movements));
	self->animation = animation;
	self->is_offscreen = false;
	self->bobdelta = 0;
	self->bobrate = 0.001;
	return self;
}

void so_del(so *self)
{
	assert(self);
	dealloc(self);
}

so *so_copy(so *other)
{
	so *self = allocate(sizeof(so));
	assert(self);
	memcpy(self, other, sizeof(so));
	return self;
}

void so_newmov(so *self, so_movement movement, float amt, bool *trigger)
{
	int i;
	assert(self);
	assert(movement);
	for (i = 0; i < MOVEMENTS_MAX; i++) {
		if (self->movements[i].movement == NULL) {
			self->movements[i].movement = movement;
			self->movements[i].amt = amt;
			self->movements[i].trigger = trigger;
			break;
		}
	}
	assert(("Too many movements", i != MOVEMENTS_MAX));
}

void so_delmov(so *self, so_movement movement)
{
	int i;
	assert(self);
	assert(movement);
	for (i = 0; i < MOVEMENTS_MAX; i++) {
		if (self->movements[i].movement == movement) {
			self->movements[i].movement = NULL;
			self->movements[i].amt = 0.0f;
			self->movements[i].trigger = NULL;
			break;
		}
	}
	assert(("Movement not found", i != MOVEMENTS_MAX));
}

void so_set_pos(so *self, float x, float y)
{
	self->pos.x = x;
	self->pos.y = y;
}


void so_set_bobrate(so *self, float newrate)
{
	assert(self);
	self->bobrate = newrate;
}

void so_draw(so *self)
{
	assert(self);
	if (self->is_offscreen) {
		return;
	}
	anim_draw(self->animation, self->pos);
}

void so_move(so *self)
{
	int i;
	assert(self);
	for (i = 0; i < MOVEMENTS_MAX; i++) {
		if (self->movements[i].movement) {
			self->movements[i].movement(self,
				self->movements[i].amt, self->movements[i].trigger);
		}
	}
}


void so_cb_left(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.x -= amt;

		if (self->pos.x + anim_get_width(self->animation) < 0) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_cb_right(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.x += amt;

		if (self->pos.x > GetScreenWidth()) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_cb_up(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.y -= amt;

		if (self->pos.y + anim_get_height(self->animation) < 0) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_cb_down(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.y += amt;

		if (self->pos.y > GetScreenHeight()) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_cb_bob_vrt(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.y += amt * sinf(self->bobdelta);
		self->bobdelta += self->bobrate;
		if (self->bobdelta > 2 * PI) {
			self->bobdelta = 0;
		}
	}
}

void so_cb_bob_hrz(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.x += amt * cosf(self->bobdelta);
		self->bobdelta += self->bobrate;
		if (self->bobdelta > 2 * PI) {
			self->bobdelta = 0;
		}
	}
}

void so_cb_loop_left(so *self, float amt, bool *trigger)
{
	so_cb_left(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.x = GetScreenWidth();
	}
}

void so_cb_loop_right(so *self, float amt, bool *trigger)
{
	so_cb_right(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.x = 0 - anim_get_width(self->animation);
	}
}

void so_cb_loop_up(so *self, float amt, bool *trigger)
{
	so_cb_up(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.y = GetScreenHeight();
	}
}

void so_cb_loop_down(so *self, float amt, bool *trigger)
{
	so_cb_down(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.y = 0 - anim_get_height(self->animation);
	}
}
