#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <raylib.h>
#include "imger.h"
#include "scene_manager.h"
#include "util.h"

/* max number of scenes to use at once */
#define ACTIVE_SCENES_MAX 16

static void scene_paragon(scene *self, void *args);
static void scene_beetle_fleet(scene *self, void *args);

static imger *img = NULL;
static scene *active_scenes[ACTIVE_SCENES_MAX] = { NULL };
static bool active_visible[ACTIVE_SCENES_MAX] = { false }; /* visibility */

/* extern, tells how to load each scene */
struct scene_cbs_tag scenes = {
	.paragon = scene_paragon,
	.beetle_fleet = scene_beetle_fleet
};

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

void scene_man_take(scene *other)
{
	int i;

	assert(("Must pass an existing scene", other != NULL));

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i] == NULL) {
			active_scenes[i] = other;
			active_visible[i] = true;
			break;
		}
	}

	assert(("Too many scenes", i != ACTIVE_SCENES_MAX));
}

void scene_man_update(void)
{
	int i;
	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i]) {
			scene_update(active_scenes[i]);
		}
	}
}

void scene_man_draw(void)
{
	int i;
	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i] && active_visible[i]) {
			scene_draw(active_scenes[i]);
		}
	}
}

void scene_man_set_visible(char *scene_name, bool is_visible)
{
	int i;
	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (!active_scenes[i]) {
			continue;
		}

		if (streq(scene_name, scene_get_name(active_scenes[i]))) {
			active_visible[i] = is_visible;
			break;
		}
	}
	assert(("Cannot find scene to set visibility", i != ACTIVE_SCENES_MAX));
}

static void scene_paragon(scene *self, void *args)
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

static void scene_beetle_fleet(scene *self, void *args)
{
	int i;
	so *tmp, *template;
	bool *trigger = (bool *)args;

	assert(img != NULL);

	template = so_new(imger_load(img, "assets/beetle-sml.png"));
	so_newmov(template, so_move_loop_up, 10, trigger);
	so_newmov(template, so_move_bob_hrz, 10, trigger);
	scene_load_object(self, template);
	for (i = 0; i < 50; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.05);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		scene_load_object(self, tmp);
	}
}
