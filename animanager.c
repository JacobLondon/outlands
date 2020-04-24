#include <assert.h>
#include <memory.h>
#include <raylib.h>
#include "anim.h"
#include "animanager.h"
#include "util.h"

#define IMGS_MAX 64

typedef struct animan_tag {
	char *pngs[IMGS_MAX];
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
		if (self->pngs[i]) {
			anim_del(self->animations[i]);
			self->pngs[i] = NULL;
		}
	}
	dealloc(self);
	*((animan **)self) = NULL;
}

void animan_update(animan *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->pngs[i] != NULL) {
			anim_update(self->animations[i]);
		}
	}
}

anim *animan_get(animan *self, char *png)
{
	int i;
	assert(self);
	assert(png);
	for (i = 0; i < IMGS_MAX; i++) {
		if (streq(self->pngs[i], png)) {
			return self->animations[i];
		}
	}
	assert(("Animation not found", i != IMGS_MAX));
	return self->animations[i];
}

void animan_load(animan *self, char *png, int width, int height)
{
	int i;
	assert(self);
	assert(png);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->pngs[i] == NULL) {
			self->pngs[i] = (char *)png;
			self->animations[i] = anim_new(png, width, height);
			break;
		}
	}
	assert(("Too many animations loaded", i != IMGS_MAX));
}
