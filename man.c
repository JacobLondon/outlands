#include <assert.h>
#include <memory.h>
#include "man.h"
#include "util.h"

#define SO_MAX 512

typedef struct man_tag {
	so *sos[SO_MAX];
} man;

man *man_new(void )
{
	man *self = allocate(sizeof(man));
	assert(self);
	memset(self->sos, 0, sizeof(self->sos));
	return self;
}

void man_del(man *self)
{
	int i;
	assert(self);
	man_scene_clear(self);
	freedom(self);
}

void man_scene_load(man *self, so *s)
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
	assert(("Too many scene objects", i != SO_MAX));
}

void man_scene_clear(man *self)
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

void man_scene_draw(man *self)
{
	int i;
	assert(self);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i]) {
			so_draw(self->sos[i]);
		}
	}
}

void man_scene_update(man *self)
{
	int i;
	assert(self);
	for (i = 0; i < SO_MAX; i++) {
		if (self->sos[i]) {
			so_move(self->sos[i]);
		}
	}
}