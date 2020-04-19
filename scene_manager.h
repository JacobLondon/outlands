#ifndef OUTLANDS_MANAGER_H_
#define OUTLANDS_MANAGER_H_

#include "scene.h"

/* All things which can be loaded.
 * Images, scenes, etc...
 */

/* Allocate imger for scene management */
void scene_man_init(void);

/* Deallocate imger */
void scene_man_cleanup(void);

/* Load all given definitions by name. Each definition
 * statically exists inside the corresponding *.c file
 * 'names' must be NULL terminated and all names MUST
 * exist */
void scene_man_load(char **names);

/* call update on all scenes */
void scene_man_update(void);

/* call draw on all scenes */
void scene_man_draw(void);

/* Bind visibility to a local variable,
 * is_visible == NULL means always draw
 * 
 * Control scene visibility at the manager level. Scenes
 * themselves (and scene objects for that matter) do not
 * know about visibility as that is not their job. They
 * just draw when told to. The manager can decide who is told. */
void scene_man_tie_visibility(char *scene_name, bool *is_visible);

#endif // OUTLANDS_MANAGER_H_
