#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <raylib.h>
#include "animanager.h"
#include "scene_manager.h"
#include "texture_manager.h"
#include "util.h"
#include "globals.h"


#define ACTIVE_SCENES_MAX 16 /* max number of scenes to use at once */
#define ANIMATION_RATE 10.0f /* animation updates per second, > 0.0 */


/* Take ownership of scene. Remove scene on unload, draw on man_draw,
 * update on man_update, etc... Set visible to true by default */
static void take_scene(scene *other);

/* Create a new scene and give it directly to the scene manager */
#define emplace_scene(Name, MaxObjects, SceneCb_Init) \
	take_scene(scene_new((Name), (MaxObjects), (SceneCb_Init))

#define emplace_scene_def(SceneDefinition) \
	take_scene(scene_new_def(SceneDefinition))


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

static bool initialized = false;
static animan *animation_man = NULL;
static scene *active_scenes[ACTIVE_SCENES_MAX] = { NULL };
static bool *active_visibility[ACTIVE_SCENES_MAX] = { false };


void scene_man_init(void)
{
	assert(initialized == false);
	animation_man = animan_new();
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

	animan_del(animation_man);
	animation_man = NULL;
	memset(active_scenes, 0, sizeof(active_scenes));
	initialized = false;
}

void scene_man_load(char **names)
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
		if (d->name == NULL) {
			assert(("Scene definition not found", 0));
		}
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
		animan_update(animation_man);
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
	assert(("Cannot find scene to set visibility", i != ACTIVE_SCENES_MAX));
}

static void take_scene(scene *other)
{
	int i;

	assert(initialized == true);
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

static void init_helper_load_at(scene *self, char *asset, float x, float y)
{
	Texture2D *t = texman_load(asset);
	anim *a = animan_load(animation_man, t, 1, 1);
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
 *     animan_load(animation_man, "assets/picture1.png", 1, 1);
 *     so *myso = so_new(animan_get(animation_man, "assets/picture1.png"));
 *     so_set_pos(myso, GetScreenWidth() / 2, GetScreenHeight() / 2);
 *     so_newmov(myso, so_cb_bob_hrz, 0.1, &global_variable_controlling_this);
 *     scene_load_object(self, myso);
 * }
 * 
 ****************************************************************************/

static void init_cb_star1(scene *self)
{
	init_helper_load_at(self, "assets/star 1.png", 0, 0);
}

static void init_cb_star2(scene *self)
{
	init_helper_load_at(self, "assets/star 2.png", 0, 0);
}

static void init_cb_star3(scene *self)
{
	init_helper_load_at(self, "assets/star 3.png", 0, 0);
}

static void init_cb_space3(scene *self)
{
	init_helper_load_at(self, "assets/space 3.png", 0, 0);
}

static void init_cb_heavy_nebula2(scene *self)
{
	init_helper_load_at(self, "assets/heavynebula 2.png", 0, 0);
}

static void init_cb_star_nebula3(scene *self)
{
	init_helper_load_at(self, "assets/starnebula 3.png", 0, 0);
}

static void init_cb_gluurus(scene *self)
{
	Texture2D *t = texman_load("assets/Gluurus.png");
	anim *a = animan_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, GetScreenWidth() * 0.1, -100);
	so_newmov(s, so_cb_bob_hrz, 0.2, NULL);
	scene_load_object(self, s);
}

static void init_cb_paragon(scene *self)
{
	Texture2D *t = texman_load("assets/Lyra Paragon.png");
	anim *a = animan_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_newmov(s, so_cb_loop_left, 0.1, NULL);
	scene_load_object(self, s);
}

static void init_cb_skyrillis(scene *self)
{
	Texture2D *t = texman_load("assets/Skyrillis.png");
	anim *a = animan_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, GetScreenWidth() * 0.15, 0);
	scene_load_object(self, s);
}

static void init_cb_reitis(scene *self)
{
	Texture2D *t = texman_load("assets/Reitis.png");
	anim *a = animan_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_altaira(scene *self)
{
	Texture2D *t = texman_load("assets/Altaira.png");
	anim *a = animan_load(animation_man, t, 1, 1);
	so *s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_beetles(scene *self)
{
	int i;
	so *tmp;

	Texture2D *beetles = texman_load("assets/beetle-sml.png");
	anim *a = animan_load(animation_man, beetles, 1, 1);
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

	asteroids[0] = texman_load("assets/asteroid0.png");
	asteroids[1] = texman_load("assets/asteroid1.png");
	asteroids[2] = texman_load("assets/asteroid2.png");
	asteroids[3] = texman_load("assets/asteroid3.png");
	asteroids[4] = texman_load("assets/asteroid4.png");
	asteroids[5] = texman_load("assets/asteroid5.png");
	asteroids[6] = texman_load("assets/asteroid6.png");
	asteroids[7] = texman_load("assets/asteroid7.png");
	asteroids[8] = texman_load("assets/asteroid8.png");
	asteroids[9] = texman_load("assets/asteroid9.png");

	for (i = 0; i < XASTEROIDS; i++) {
		an = animan_load(animation_man, asteroids[i], 1, 1);
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
	Texture2D *t = texman_load("assets/executive.png");
	anim *a = animan_load(animation_man, t, 1, 1);
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
