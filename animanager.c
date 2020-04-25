#include <assert.h>
#include <memory.h>
#include "anim.h"
#include "animanager.h"
#include "texture_manager.h"
#include "util.h"

#define IMGS_MAX 64

typedef struct animan_tag {
	Texture2D *textures[IMGS_MAX];
	anim *animations[IMGS_MAX];
} animan;

animan *animan_new(void)
{
	animan *self = allocate(sizeof(animan));
	assert(self);
	memset(self, 0, sizeof(animan));
	return self;
}

void animan_del(animan *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i]) {
			anim_del(self->animations[i]);
			self->textures[i] = NULL;
		}
	}
	dealloc(self);
}

void animan_update(animan *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] != NULL) {
			anim_update(self->animations[i]);
		}
	}
}

anim *animan_get(animan *self, Texture2D *texture)
{
	int i;
	assert(self);
	assert(texture);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] == texture) {
			return self->animations[i];
		}
	}
	assert(("Animation not found", i != IMGS_MAX));
	return self->animations[i];
}

void animan_load(animan *self, Texture2D *texture, int width, int height)
{
	int i;
	assert(self);
	assert(texture);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] == NULL) {
			self->textures[i] = texture;
			self->animations[i] = anim_new(texture, width, height);
			break;
		}
		// texture already exists, don't need to load it again
		else if (self->textures[i] == texture) {
			break;
		}
	}
	assert(("Too many animations loaded", i != IMGS_MAX));
}
