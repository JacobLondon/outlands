#ifndef OUTLANDS_SCENE_H
#define OUTLANDS_SCENE_H

#include "scene_object.h"

/* Manage groupings of scene objects. Call update and
 * draw on many scene objects at once for convenience.
 * Scene objects can still be manually managed however.
 */

typedef struct scene_tag scene;
typedef void (*scene_cb)(struct scene_tag *self);


/* Args to pass into scene_new. Can define
 * a scene without loading until 'new' is
 * called. An additional way to create a
 * scene */
typedef struct scene_definition_tag {
	char *name;
	size_t max_objects;
	scene_cb init;
} scene_definition;

scene *scene_new(char *name, size_t max_objects, scene_cb init);
scene *scene_new_def(scene_definition *def);
void scene_del(scene *self);

/* Give ownership to the scene */
void scene_load_object(scene *self, so *s);
/* Free all objects in the scene */
void scene_clear(scene *self);
/* Call draw on all objects in the scene */
void scene_draw(scene *self);
/* Call move on all objects in the scene */
void scene_update(scene *self);

char *scene_get_name(scene *self);

#endif // OUTLANDS_SCENE_H
