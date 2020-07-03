#include <memory.h>
#include <stddef.h>
#include <raylib.h>
#include "anim_man.h"
#include "scene_man.h"
#include "texture_man.h"
#include "util.h"
#include "globals.h"


#define ACTIVE_SCENES_MAX 16 /* max number of scenes to use at once */
#define ANIMATION_RATE 10.0f /* animation updates per second, > 0.0 */

/**
 * Take ownership of scene. Remove scene on unload, draw on man_draw,
 * update on man_update, etc... Set visible to true by default
 */
static void take_scene(scene *other);

/* Create a new scene and give it directly to the scene manager */
#define emplace_scene(Name, MaxObjects, SceneCb_Init) \
	take_scene(scene_new((Name), (MaxObjects), (SceneCb_Init))

#define emplace_scene_def(SceneDefinition) \
	take_scene(scene_new_def(SceneDefinition))

typedef struct scene_set_tag {
	const char *name;
	char *scene_names[ACTIVE_SCENES_MAX];
} scene_set;

static void init_cb_star1(scene *self);
static void init_cb_star2(scene *self);
static void init_cb_star3(scene *self);
static void init_cb_space3(scene *self);
static void init_cb_heavy_nebula2(scene *self);
static void init_cb_star_nebula3(scene *self);

static void init_cb_gluurus(scene *self);
static void init_cb_paragon(scene *self);
static void init_cb_skyrillis(scene *self);
static void init_cb_reitis(scene *self);
static void init_cb_altaira(scene *self);

static void init_cb_beetles(scene *self);
static void init_cb_executives(scene *self);
static void init_cb_asteroids(scene *self);

/* load a single asset into a scene at the coordinates */
static void init_helper_load_at(scene *self, char *asset, float x, float y);
static void load_names(char **names);

static scene_definition defs[] = {
	// Background
	{ "Star1", 1, init_cb_star1 },
	{ "Star2", 1, init_cb_star2 },
	{ "Star3", 1, init_cb_star3 },
	{ "Space3", 1, init_cb_space3 },
	{ "Heavy Nebula 2", 1, init_cb_heavy_nebula2 },
	{ "Star Nebula 3", 1, init_cb_star_nebula3 },
	// Planets
	{ "Gluurus", 1, init_cb_gluurus },
	{ "Paragon", 1, init_cb_paragon },
	{ "Skyrillis", 1, init_cb_skyrillis },
	{ "Reitis", 1, init_cb_reitis },
	{ "Altaira", 1, init_cb_altaira },
	// Foreground
	{ "Beetles", 200, init_cb_beetles },
	{ "Asteroids", 400, init_cb_asteroids },
	{ "Executives", 3, init_cb_executives },
	{ NULL, 0, NULL }
};

/**
 * Map a set name to a grouping of scenes, ordered from back to front, left to right
 * NULL term the initializer list
 */
static scene_set set_definitions[] = {
	{ "Gluurus", { "Star Nebula 3", "Gluurus", "Beetles", NULL } },
	{ "Skyrillis", { "Star1", "Skyrillis", "Asteroids", NULL } },
	{ "Reitis", { "Space3", "Reitis", "Executives", NULL } },
	{ "Paragon", { "Star3", "Paragon", NULL } },
	{ "Altaira", { "Star2", "Altaira", NULL } },
};

static size_t set_count = ARRAY_SIZE(set_definitions);
static int set_loaded_idx = 0;
static bool initialized = false;
static anim_man *animation_man = NULL;
static scene *active_scenes[ACTIVE_SCENES_MAX] = { NULL };
static bool *active_visibility[ACTIVE_SCENES_MAX] = { false };


void scene_man_init(void)
{
	assert(initialized == false);
	animation_man = anim_man_new();
	assert(animation_man);
	memset(active_scenes, 0, sizeof(active_scenes));
	initialized = true;
}

void scene_man_cleanup(void)
{
	int i;

	assert(initialized == true);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i]) {
			scene_del(active_scenes[i]);
		}
	}

	anim_man_del(animation_man);
	animation_man = NULL;
	memset(active_scenes, 0, sizeof(active_scenes));
	initialized = false;
}

void scene_man_load_set(char *name)
{
	int i;
	assert(name);
	for (i = 0; i < set_count; i++) {
		if (streq((char *)set_definitions[i].name, name)) {
			load_names(set_definitions[i].scene_names);
			set_loaded_idx = i;
			break;
		}
	}
	msg_assert(0, "Scene set named %s not found", name);
}

void scene_man_load_idx(int idx)
{
	assert(0 <= idx && idx < set_count);
	load_names(set_definitions[idx].scene_names);
	set_loaded_idx = idx;
}

void scene_man_load_rand(void)
{
	int idx = rand_range(0, set_count);
	load_names(set_definitions[idx].scene_names);
	set_loaded_idx = idx;

}

static void load_names(char **names)
{
	scene_definition *d;
	int i;
	assert(names != NULL);
	assert(initialized == true);

	for (i = 0; names[i] != NULL; i++) {
		for (d = defs; d->name; d++) {
			if (streq(names[i], d->name)) {
				emplace_scene_def(d);
				break;
			}
		}

		/* should not get to the end of defs */
		msg_assert(d->name != NULL, "Scene definition not found");
	}
}

void scene_man_update(void)
{
	int i;
	static float oof = 0;

	assert(initialized == true);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		// only update the scene if it's visible (NULL or deref to true means visible)
		if (active_scenes[i] && (active_visibility[i] == NULL || *active_visibility[i])) {
			scene_update(active_scenes[i]);
		}
	}

	if (oof > 1.0f / ANIMATION_RATE) {
		anim_man_update(animation_man);
		oof = 0.0f;
	}
	oof += GetFrameTime();
}

void scene_man_draw(void)
{
	int i;

	assert(initialized == true);

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

	assert(initialized == true);
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
	msg_assert(i != ACTIVE_SCENES_MAX, "Cannot find scene %s to set visibility", scene_name);
}

static void take_scene(scene *other)
{
	int i;

	assert(initialized == true);
	assert(other);

	for (i = 0; i < ACTIVE_SCENES_MAX; i++) {
		if (active_scenes[i] == NULL) {
			active_scenes[i] = other;
			active_visibility[i] = NULL;
			break;
		}
	}

	msg_assert(i != ACTIVE_SCENES_MAX, "Too many scenes: %d", i);
}

static void init_helper_load_at(scene *self, char *asset, float x, float y)
{
	Texture2D *t = texture_man_load(asset);
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, x, y);
	scene_load_object(self, s);
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
 * Also, 'initialized' should have already been asserted to true.
 * 
 * example:
 * 
 * static void init_cb_picture1(scene *self)
 * {
 *     // load a picture with one animation frame, 1 * 1 = 1
 *     anim_man_load(animation_man, ASSET_DIRECTORY "/picture1.png", 1, 1);
 *     so *myso = so_new(anim_man_get(animation_man, ASSET_DIRECTORY "/picture1.png"));
 *     so_set_pos(myso, GetScreenWidth() / 2, GetScreenHeight() / 2);
 *     so_newmov(myso, so_cb_bob_hrz, 0.1, &global_variable_controlling_this);
 *     scene_load_object(self, myso);
 * }
 * 
 ****************************************************************************/

static void init_cb_star1(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/star 1.png", 0, 0);
}

static void init_cb_star2(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/star 2.png", 0, 0);
}

static void init_cb_star3(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/star 3.png", 0, 0);
}

static void init_cb_space3(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/space 3.png", 0, 0);
}

static void init_cb_heavy_nebula2(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/heavynebula 2.png", 0, 0);
}

static void init_cb_star_nebula3(scene *self)
{
	init_helper_load_at(self, ASSET_DIRECTORY "/starnebula 3.png", 0, 0);
}

static void init_cb_gluurus(scene *self)
{
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/Gluurus.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, GetScreenWidth() * 0.1, -100);
	so_newmov(s, so_cb_bob_hrz, 0.2, NULL);
	scene_load_object(self, s);
}

static void init_cb_paragon(scene *self)
{
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/Lyra Paragon.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_newmov(s, so_cb_loop_left, 0.1, NULL);
	scene_load_object(self, s);
}

static void init_cb_skyrillis(scene *self)
{
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/Skyrillis.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, GetScreenWidth() * 0.15, 0);
	scene_load_object(self, s);
}

static void init_cb_reitis(scene *self)
{
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/Reitis.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_altaira(scene *self)
{
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/Altaira.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_beetles(scene *self)
{
	int i;
	so *tmp;

	Texture2D *beetles = texture_man_load(ASSET_DIRECTORY "/beetle-sml.png");
	anim *a = anim_man_load(animation_man, beetles, 1, 1);
	so *template = so_new(a);

	so_newmov(template, so_cb_loop_up, 10, NULL);
	so_newmov(template, so_cb_bob_hrz, 10, NULL);
	scene_load_object(self, template);
	for (i = 0; i < 50; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.05);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		scene_load_object(self, tmp);
	}
}

static void init_cb_asteroids(scene *self)
{
	#define XASTEROIDS 10
	int i, j;
	so *tmp;
	so *template;
	anim *an;
	Texture2D *asteroids[XASTEROIDS];

	int direction = rand_range(0, 2); // left or right
	float speed = rand_uniform() * 3;
	int num_asteroids = rand_range(10, 20); // asteroids per type

	asteroids[0] = texture_man_load(ASSET_DIRECTORY "/asteroid0.png");
	asteroids[1] = texture_man_load(ASSET_DIRECTORY "/asteroid1.png");
	asteroids[2] = texture_man_load(ASSET_DIRECTORY "/asteroid2.png");
	asteroids[3] = texture_man_load(ASSET_DIRECTORY "/asteroid3.png");
	asteroids[4] = texture_man_load(ASSET_DIRECTORY "/asteroid4.png");
	asteroids[5] = texture_man_load(ASSET_DIRECTORY "/asteroid5.png");
	asteroids[6] = texture_man_load(ASSET_DIRECTORY "/asteroid6.png");
	asteroids[7] = texture_man_load(ASSET_DIRECTORY "/asteroid7.png");
	asteroids[8] = texture_man_load(ASSET_DIRECTORY "/asteroid8.png");
	asteroids[9] = texture_man_load(ASSET_DIRECTORY "/asteroid9.png");

	for (i = 0; i < XASTEROIDS; i++) {
		an = anim_man_load(animation_man, asteroids[i], 1, 1);
		template = so_new(an);

		for (j = 0; j < num_asteroids; j++) {
			tmp = so_copy(template);

			switch (direction) {
			case 0:
				so_newmov(tmp, so_cb_loop_left, speed + rand_uniform() * XASTEROIDS, NULL);
				break;
			case 1:
				so_newmov(tmp, so_cb_loop_right, speed + rand_uniform() * XASTEROIDS, NULL);
				break;
			}
			so_set_pos(tmp, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());

			switch (rand_range(0, 4)) {
			// bob up down
			case 0:
				so_newmov(tmp, so_cb_bob_vrt, 2, NULL);
				break;
			// slight upwards
			case 1:
				so_newmov(tmp, so_cb_loop_up, 2, NULL);
				break;
			// slight downwards
			case 2:
				so_newmov(tmp, so_cb_loop_down, 2, NULL);
				break;
			// horizontal
			case 3:
				break;
			}
			scene_load_object(self, tmp);
		}
	}

	#undef XASTEROIDS
}

static void init_cb_executives(scene *self)
{
	int i;
	so *tmp;
	Texture2D *t = texture_man_load(ASSET_DIRECTORY "/executive.png");
	anim *a = anim_man_load(animation_man, t, 1, 1);
	so *template = so_new(a);
	so_newmov(template, so_cb_bob_vrt, 0.1, NULL);
	so_set_bobrate(template, 0.05);
	for (i = 0; i < 3; i++) {
		tmp = so_copy(template);
		so_set_pos(tmp,
			GetScreenWidth() / 2 + GetScreenWidth() * 0.3 * rand_uniform(),
			GetScreenHeight() / 10 + GetScreenHeight() * 0.5 * rand_uniform()
		);
		scene_load_object(self, tmp);
	}
}
