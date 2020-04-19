#ifndef OUTLANDS_LOAD_H_
#define OUTLANDS_LOAD_H_

#include "scene.h"

/* All things which can be loaded.
 * Images, scenes, etc...
 */

/* Allocate imger for scene management */
void scene_man_init(void);
/* Deallocate imger */
void scene_man_cleanup(void);

/* Take ownership of scene. Remove scene on unload, draw on man_draw,
 * update on man_update, etc... Set visible to true by default*/
void scene_man_take(scene *other);
/* Create a new scene and give it directly to the scene manager */
#define scene_man_emplace(Name, MaxObjects, SceneCb_Init, CbArgs) \
	scene_man_take(scene_new((Name), (MaxObjects), (SceneCb_Init), (CbArgs)))

/* call update on all scenes */
void scene_man_update(void);
/* call draw on all scenes */
void scene_man_draw(void);

void scene_man_set_visible(char *scene_name, bool is_visible);

struct scene_cbs_tag {
	scene_cb paragon;
	scene_cb beetle_fleet;
};

extern struct scene_cbs_tag scenes;

#endif // OUTLANDS_LOAD_H_
