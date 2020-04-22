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
static void take_scene(scene *other);

/* Create a new scene and give it directly to the scene manager */
#define emplace_scene(Name, MaxObjects, SceneCb_Init) \
	take_scene(scene_new((Name), (MaxObjects), (SceneCb_Init))

#define emplace_scene_def(SceneDefinition) \
	take_scene(scene_new_def(SceneDefinition))


static void init_cb_paragon(scene *self);
static void init_cb_beetle_fleet(scene *self);
static void init_cb_explosion(scene *self);

static scene_definition defs[] = {
	{ "Paragon", 3, init_cb_paragon },
	{ "Beetles", 200, init_cb_beetle_fleet },
	{ "Explosion", 1, init_cb_explosion },
	{ NULL, 0, NULL }
};


static imger *img = NULL;
static scene *active_scenes[ACTIVE_SCENES_MAX] = { NULL };
static bool *active_visibility[ACTIVE_SCENES_MAX] = { false };


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
	assert(names != NULL);
	assert(img != NULL);

	for (i = 0; names[i] != NULL; i++) {
		for (d = defs; d->name; d++) {
			if (streq(names[i], d->name)) {
				emplace_scene_def(d);
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
	static unsigned int oof = 0;

	assert(img != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		// only update the scene if it's visible
		if (active_scenes[i] && (active_visibility[i] == NULL || *active_visibility[i])) {
			scene_update(active_scenes[i]);
		}
	}

	if (oof % 4 == 0) {
		imger_update(img);
		oof = 0;
	}
	oof++;
}

void scene_man_draw(void)
{
	int i;

	assert(img != NULL);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (!active_scenes[i]) {
			continue;
		}

		// only draw the scene if it's visible
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

static void take_scene(scene *other)
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
 * loaded into 'definitions'. These callbacks should create
 * 'so' or 'screen objects'. After each screen object is created with
 * 'so_newmov' / 'so_set_pos' / etc... Call 'scene_load_object(self, YOUR SO)'
 * This will properly insert a screen object into the system.
 * 
 * example:
 * 
 * static void init_cb_picture1(scene *self)
 * {
 *     // note that imger_get gets the Texture2D whether it was alloc'ed before or not
 *     so *myso = so_new(imger_get(img, "assets/picture1.png"));
 *     so_set_pos(myso, GetScreenWidth() / 2, GetScreenHeight() / 2);
 *     so_newmov(myso, so_cb_bob_hrz, 0.1, &global_variable_controlling_this);
 *     scene_load_object(self, myso);
 * }
 * 
 ****************************************************************************/


static void init_cb_paragon(scene *self)
{
	so *tmp;

	assert(img != NULL);

	imger_load(img, "assets/star 3.png", 1, 1);
	imger_load(img, "assets/Lyra Paragon.png", 1, 1);

	tmp = so_new(imger_get(img, "assets/star 3.png"));
	so_set_pos(tmp, 0, 0);
	scene_load_object(self, tmp);

	tmp = so_new(imger_get(img, "assets/Lyra Paragon.png"));
	so_newmov(tmp, so_cb_loop_left, 0.1, NULL);
	scene_load_object(self, tmp);
}

static void init_cb_beetle_fleet(scene *self)
{
	int i;
	so *tmp, *template;

	assert(img != NULL);

	imger_load(img, "assets/beetle-sml.png", 1, 1);

	template = so_new(imger_get(img, "assets/beetle-sml.png"));
	so_newmov(template, so_cb_loop_up, 10, &beetles_launch);
	so_newmov(template, so_cb_bob_hrz, 10, &beetles_launch);
	scene_load_object(self, template);
	for (i = 0; i < 50; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.05);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		scene_load_object(self, tmp);
	}
}

static void init_cb_explosion(scene *self)
{
	so *tmp;
	assert(img != NULL);

	imger_load(img, "assets/explosion.png", 4, 4);
	tmp = so_new(imger_get(img, "assets/explosion.png"));
	so_set_pos(tmp, 300, 300);
	so_set_bobrate(tmp, 0.1);
	so_newmov(tmp, so_cb_bob_hrz, 4, NULL);
	so_newmov(tmp, so_cb_bob_vrt, 4, NULL);
	scene_load_object(self, tmp);
}
