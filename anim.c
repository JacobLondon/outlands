#include <assert.h>
#include <memory.h>
#include "anim.h"
#include "util.h"

typedef struct anim_tag {
	Texture2D *texture;
	int width;
	int height;
	int i;
	int j;
} anim;


anim *anim_new(Texture2D *texture, int width, int height)
{
	anim *self = allocate(sizeof(anim));
	assert(texture);
	assert(self);
	self->texture = texture;
	self->width = width;
	self->height = height;
	self->i = 0;
	self->j = 0;
	return self;
}

void anim_del(anim *self)
{
	assert(self);
	memset(self, 0, sizeof(anim));
	dealloc(self);
}

anim *anim_copy(anim *self)
{
	assert(self);
	anim *other = allocate(sizeof(anim));
	assert(other);
	memcpy(other, self, sizeof(anim));
	anim_reset(other);
	return other;
}

void anim_reset(anim *self)
{
	assert(self);
	self->i = 0;
	self->j = 0;
}

void anim_update(anim *self)
{
	assert(self);

	self->j++;
	
	if (self->j >= self->width) {
		self->j = 0;
		self->i++;
	}

	if (self->i >= self->height) {
		self->i = 0;
	}
}

void anim_draw(anim *self, Vector2 pos)
{
	assert(self);
	DrawTextureRec(
		*self->texture,
		(Rectangle){
			self->texture->width / self->width * self->j,
			self->texture->height / self->height * self->i,
			self->texture->width / self->width,
			self->texture->height / self->height
		},
		pos, WHITE
	);
}

int anim_get_width(anim *self)
{
	assert(self);
	return self->texture->width / self->width;
}

int anim_get_height(anim *self)
{
	assert(self);
	return self->texture->height / self->height;
}
