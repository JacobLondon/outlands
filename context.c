#include <assert.h>
#include <stddef.h>
#include "scene_manager.h"
#include "texture_manager.h"
#include "key_manager.h"
#include "ship.h"
#include "globals.h"
#include "util.h"
#include "context.h"

// copied or unique key objects
#define KEYS_MAX 32
#define SCENES_MAX 3
#define SHIPS_MAX 32
#define POOL_MAX 100 /* kilobytes for visuals management */


static char *scene_defs[SCENES_MAX][] = {
	{ "Star Nebula 3", "Gluurus", "Beetles", NULL },
	{ "Star1", "Skyrillis", "Asteroids", NULL },
	{ "Space3", "Reitis", "Executives", NULL },
};

static char *key_defs[] = {
	"Missile", NULL
};

static ship *player_ship = NULL;

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

	player_ship = ship_new("Falcon");
	pool_usage();
}

static void def_cleanup(void)
{
	ship_del(player_ship);
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
	ship_update(player_ship);
}

void context_draw(void)
{
	scene_man_draw();
	ship_draw(player_ship);
	key_man_update();
	DrawFPS(0, 0);
}
