#include <assert.h>
#include <memory.h>
#include "scene.h"
#include "util.h"

typedef struct scene_tag {
	so **scene_objects;
	scene_cb init;
	char *name;
	size_t max_objects;
} scene;

scene *scene_new(char *name, size_t max_objects, scene_cb init, void *cb_args)
{
	scene *self = allocate(sizeof(scene));
	assert(self);
	self->scene_objects = allocate(sizeof(so *) * max_objects);
	memset(self->scene_objects, 0, sizeof(self->scene_objects));
	self->init = init;
	self->name = name;
	self->max_objects = max_objects;

	if (self->init) {
		self->init(self, cb_args);
	}
	return self;
}

void scene_del(scene *self)
{
	int i;
	assert(self);
	scene_clear(self);
	freedom(self);
}

void scene_load_object(scene *self, so *s)
{
	int i;
	assert(self);
	assert(s);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i] == NULL) {
			self->scene_objects[i] = s;
			break;
		}
	}
	assert(("Too many scene objects", i != self->max_objects));
}

void scene_clear(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_del(self->scene_objects[i]);
			self->scene_objects[i] = NULL;
		}
	}
}

void scene_draw(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_draw(self->scene_objects[i]);
		}
	}
}

void scene_update(scene *self)
{
	int i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_move(self->scene_objects[i]);
		}
	}
}

char *scene_get_name(scene *self)
{
	return self->name;
}
