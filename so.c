#include <assert.h>
#include <math.h>
#include <memory.h>
#include <raylib.h>
#include "so.h"
#include "util.h"

#define MOVEMENTS_MAX 4

typedef struct so_tag {
	Vector2 pos;
	float bobrate;
	struct {
		so_movement movement;
		bool *trigger;
		float amt;
	} movements[MOVEMENTS_MAX];
	Texture2D *texture;
	bool is_offscreen;
} so;


so *so_new(Texture2D *png)
{
	assert(png);
	so *self = allocate(sizeof(so));
	assert(self);
	self->pos = (Vector2){GetScreenWidth() / 2, 0};
	memset(self->movements, 0, sizeof(self->movements));
	self->texture = png;
	self->is_offscreen = false;
	self->bobrate = 0.001;
	return self;
}

void so_del(so *self)
{
	assert(self);
	freedom(self);
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
	DrawTextureRec(
		*self->texture,
		(Rectangle){0.0f, 0.0f, self->texture->width, self->texture->height},
		self->pos, WHITE);
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


void so_move_left(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.x -= amt;

		if (self->pos.x + self->texture->width < 0) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_move_right(so *self, float amt, bool *trigger)
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

void so_move_up(so *self, float amt, bool *trigger)
{
	if (trigger == NULL || *trigger == true) {
		self->pos.y -= amt;

		if (self->pos.y + self->texture->height < 0) {
			self->is_offscreen = true;
		}
		else {
			self->is_offscreen = false;
		}
	}
}

void so_move_down(so *self, float amt, bool *trigger)
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

void so_move_bob_vrt(so *self, float amt, bool *trigger)
{
	static float delta = 0;
	if (trigger == NULL || *trigger == true) {
		self->pos.y += amt * sinf(delta);
		delta += self->bobrate;
		if (delta > 2 * PI) {
			delta = 0;
		}
	}
}

void so_move_bob_hrz(so *self, float amt, bool *trigger)
{
	static float delta = 0;
	if (trigger == NULL || *trigger == true) {
		self->pos.x += amt * cosf(delta);
		delta += self->bobrate;
		if (delta > 2 * PI) {
			delta = 0;
		}
	}
}

void so_move_loop_left(so *self, float amt, bool *trigger)
{
	so_move_left(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.x = GetScreenWidth();
	}
}

void so_move_loop_right(so *self, float amt, bool *trigger)
{
	so_move_right(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.x = 0 - self->texture->width;
	}
}

void so_move_loop_up(so *self, float amt, bool *trigger)
{
	so_move_up(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.y = GetScreenHeight();
	}
}

void so_move_loop_down(so *self, float amt, bool *trigger)
{
	so_move_down(self, amt, trigger);
	if (self->is_offscreen) {
		self->pos.y = 0 - self->texture->height;
	}
}
