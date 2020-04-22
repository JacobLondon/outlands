#include <assert.h>
#include <memory.h>
#include <raylib.h>
#include "anim.h"
#include "imger.h"
#include "util.h"

#define IMGS_MAX 64

typedef struct imger_tag {
	char *pngs[IMGS_MAX];
	anim *animations[IMGS_MAX];
} imger;

imger *imger_new(void)
{
	imger *self = allocate(sizeof(imger));
	assert(self);
	memset(self, 0, sizeof(imger));
	return self;
}

void imger_del(imger *self)
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
	*((imger **)self) = NULL;
}

void imger_update(imger *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->pngs[i] != NULL) {
			anim_update(self->animations[i]);
		}
	}
}

anim *imger_get(imger *self, char *png)
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

void imger_load(imger *self, char *png, int width, int height)
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
