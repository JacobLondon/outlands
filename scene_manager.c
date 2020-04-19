#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <raylib.h>
#include "imger.h"
#include "scene_manager.h"
#include "util.h"
#include "globals.h"

/* max number of scenes to use at once */
#define ACTIVE_SCENES_MAX 16


/* Take ownership of scene. Remove scene on unload, draw on man_draw,
 * update on man_update, etc... Set visible to true by default */
static void take(scene *other);

/* Create a new scene and give it directly to the scene manager */
#define emplace(Name, MaxObjects, SceneCb_Init) \
	take(scene_new((Name), (MaxObjects), (SceneCb_Init))

#define emplace_def(SceneDefinition) \
	take(scene_new_def(SceneDefinition))


static void init_paragon_cb(scene *self);
static void init_beetle_fleet_cb(scene *self);

static scene_definition defs[] = {
	{ "Paragon", 3, init_paragon_cb },
	{ "Beetles", 200, init_beetle_fleet_cb },
	{ NULL, 0, NULL }
};


static imger *img = NULL;
static scene *active_scenes[ACTIVE_SCENES_MAX] = { NULL };
static bool *active_visibility[ACTIVE_SCENES_MAX] = { false }; /* visibility */


void scene_man_init(void)
{
	assert(img == NULL);
	img = imger_new();
	memset(active_scenes, 0, sizeof(active_scenes));
}

void scene_man_cleanup(void)
{
	int i;

	assert(img != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i]) {
			scene_del(active_scenes[i]);
		}
	}

	imger_del(img);
	memset(active_scenes, 0, sizeof(active_scenes));
}

void scene_man_load(char **names)
{
	scene_definition *d;
	int i;

	for (i = 0; names[i] != NULL; i++) {
		for (d = defs; d->name; d++) {
			if (streq(names[i], d->name)) {
				emplace_def(d);
				break;
			}
		}

		/* should not get to the end of defs */
		if (d->name == NULL) {
			assert(("Scene definition not found", 0));
		}
	}
}

void scene_man_update(void)
{
	int i;

	assert(img != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i]) {
			scene_update(active_scenes[i]);
		}
	}
}

void scene_man_draw(void)
{
	int i;

	assert(img != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (!active_scenes[i]) {
			continue;
		}

		if (active_visibility[i] == NULL || *active_visibility[i]) {
			scene_draw(active_scenes[i]);
		}
	}
}

void scene_man_tie_visibility(char *scene_name, bool *is_visible)
{
	int i;

	assert(img != NULL);
	assert(scene_name != NULL);
	assert(is_visible != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (!active_scenes[i]) {
			continue;
		}

		if (streq(scene_name, scene_get_name(active_scenes[i]))) {
			active_visibility[i] = is_visible;
			break;
		}
	}
	assert(("Cannot find scene to set visibility", i != ACTIVE_SCENES_MAX));
}

static void take(scene *other)
{
	int i;

	assert(img != NULL);
	assert(("Must pass an existing scene", other != NULL));

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i] == NULL) {
			active_scenes[i] = other;
			active_visibility[i] = NULL;
			break;
		}
	}

	assert(("Too many scenes", i != ACTIVE_SCENES_MAX));
}


/*****************************************************************************
 * 
 * Scene Callback Definitions
 * 
 * Scene init callbacks defined here. Ensure each callback is statically
 * loaded into 'definitions'
 * 
 ****************************************************************************/


static void init_paragon_cb(scene *self)
{
	so *tmp;

	assert(img != NULL);

	tmp = so_new(imger_load(img, "assets/star 3.png"));
	so_set_pos(tmp, 0, 0);
	scene_load_object(self, tmp);

	tmp = so_new(imger_load(img, "assets/Lyra Paragon.png"));
	so_newmov(tmp, so_move_loop_left, 0.1, NULL);
	scene_load_object(self, tmp);
}

static void init_beetle_fleet_cb(scene *self)
{
	int i;
	so *tmp, *template;

	assert(img != NULL);

	template = so_new(imger_load(img, "assets/beetle-sml.png"));
	so_newmov(template, so_move_loop_up, 10, &beetles_launch);
	so_newmov(template, so_move_bob_hrz, 10, &beetles_launch);
	scene_load_object(self, template);
	for (i = 0; i < 50; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.05);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		scene_load_object(self, tmp);
	}
}
