#include <assert.h>
#include <memory.h>
#include "scene.h"
#include "util.h"

#define SO_MAX 512

typedef struct scene_tag {
	so *sos[SO_MAX];
	char *name;
	void (*init)(struct scene_tag *self);
	void (*cleanup)(struct scene_tag *self);
} scene;


// TODO: Make inits and cleanups for different scenes
scene *scene_new(char *name, scenectlr init, scenectlr cleanup)
{
	scene *self = allocate(sizeof(scene));
	assert(self);
	memset(self->sos, 0, sizeof(self->sos));
	self->name = name;
	self->init = init;
	self->cleanup = cleanup;

	if (self->init) {
		self->init(self);
	}
	return self;
}

void scene_del(scene *self)
{
	int i;
	assert(self);
	scene_clear(self);
	if (self->cleanup) {
		self->cleanup(self);
	}
	freedom(self);
}

void scene_load_so(scene *self, so *s)
{
	int i;
	assert(self);
	assert(s);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i] == NULL) {
			self->sos[i] = s;
			break;
		}
	}
	assert(("Too sceney scene objects", i != SO_MAX));
}

void scene_clear(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i]) {
			so_del(self->sos[i]);
			self->sos[i] = NULL;
		}
	}
}

void scene_draw(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i]) {
			so_draw(self->sos[i]);
		}
	}
}

void scene_update(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i]) {
			so_move(self->sos[i]);
		}
	}
}
