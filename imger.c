#include <assert.h>
#include <memory.h>
#include <raylib.h>
#include "imger.h"
#include "util.h"

#define IMGS_MAX 64

typedef struct imger_tag {
	char *pngs[IMGS_MAX];
	Texture2D textures[IMGS_MAX];
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
			UnloadTexture(self->textures[i]);
			memset(&self->textures[i], 0, sizeof(Texture2D));
			self->pngs[i] = NULL;
		}
	}
	dealloc(self);
	*((imger **)self) = NULL;
}

Texture2D *imger_get(imger *self, const char *png)
{
	int i;
	assert(self);
	assert(png);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->pngs[i] == png) {
			return &self->textures[i];
		}
		else if (self->pngs[i] == NULL) {
			self->pngs[i] = (char *)png;
			self->textures[i] = LoadTexture(png);
			break;
		}
	}
	assert(("Too many textures loaded", i != IMGS_MAX));
	return &self->textures[i];
}
