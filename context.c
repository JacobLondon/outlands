#include <assert.h>
#include <stddef.h>
#include "scene_manager.h"
#include "texture_manager.h"
#include "key_manager.h"
#include "globals.h"
#include "util.h"
#include "context.h"

// copied or unique key objects
#define KEYS_MAX 32
#define SCENES_MAX 3
#define POOL_MAX 100 /* kilobytes for visuals management */


static char *scene_defs[SCENES_MAX][] = {
	{ "Star Nebula 3", "Gluurus", "Beetles", "Falcon Ext", "Falcon Int", NULL },
	{ "Star1", "Skyrillis", "Asteroids", "Falcon Ext", "Falcon Int", NULL },
	{ "Space3", "Reitis", "Executives", "Falcon Ext", "Falcon Int", NULL },
};

static char *key_defs[] = {
	"Missile", NULL
};

static char **loaded_scene = NULL;
static char *loaded_keys[KEYS_MAX] = { NULL };


static void def_init(void);
static void def_cleanup(void);
static void def_select(int idx);

/* 
 * Static Function Definitions
 */

static void def_init(void)
{
	pool_init(POOL_MAX);
	texman_init();

	scene_man_init();
	def_select(rand_range(0, SCENES_MAX));
	scene_man_load(loaded_scene);

	key_man_init();
	key_man_load(key_defs);

	pool_usage();
}

static void def_cleanup(void)
{
	key_man_cleanup();
	scene_man_cleanup();
	texman_cleanup();
	pool_cleanup();
}

static void def_select(int idx)
{
	assert(idx >= 0 && idx < SCENES_MAX);
	loaded_scene = scene_defs[idx];
}

/* 
 * Public Function Definitions
 */

void context_init(void)
{
	def_init();
}

void context_cleanup(void)
{
	def_cleanup();
}

void context_update(void)
{
	if (IsKeyPressed(KEY_R)) {
		def_cleanup();
		def_init();
	}
	scene_man_update();
}

void context_draw(void)
{
	scene_man_draw();
	key_man_update();
	DrawFPS(0, 0);
}
