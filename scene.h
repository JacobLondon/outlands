#ifndef OUTLANDS_SCENE_H_
#define OUTLANDS_SCENE_H_

#include "so.h"

typedef struct scene_tag scene;
typedef void (*scenectlr)(struct scene_tag *self);

scene *scene_new(char *name, scenectlr init, scenectlr cleanup);
void scene_del(scene *self);

/* Give ownership to the scene */
void scene_load_so(scene *self, so *s);
/* Free all scene objects */
void scene_clear(scene *self);
/* Call draw on all scene objects */
void scene_draw(scene *self);
/* Call move on all scene objects */
void scene_update(scene *self);

#endif // OUTLANDS_SCENE_H_