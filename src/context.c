#include <assert.h>
#include <stddef.h>
#include "scene_manager.h"
#include "texture_manager.h"
#include "key_manager.h"
#include "astar.h"
#include "ship_manager.h"
#include "dude.h"
#include "globals.h"
#include "util.h"
#include "context.h"
#include "commander.h"

// copied or unique key objects
#define KEYS_MAX 32
#define SCENES_MAX 5
#define SHIPS_MAX 32
#define POOL_MAX 60 /* kilobytes for visuals management, also the map size of nodes for A* is like 150kB */


static char *scene_defs[SCENES_MAX][] = {
	{ "Star Nebula 3", "Gluurus", "Beetles", NULL },
	{ "Star1", "Skyrillis", "Asteroids", NULL },
	{ "Space3", "Reitis", "Executives", NULL },
	{ "Star3", "Paragon", NULL },
	{ "Star2", "Altaira", NULL },
};

static char *key_defs[] = {
	"Missile", NULL
};

static char *ship_defs[] = {
	"Falcon", NULL
};

static char **loaded_scene = NULL;
static char *loaded_keys[KEYS_MAX] = { NULL };

/**
 * TODO: Config file describing current player setup
 */

static void def_init(void);
static void def_cleanup(void);
static bool player_ship_astar_cb(int i, int j);

/*
 * Static Function Definitions
 */

static bool player_ship_astar_cb(int i, int j)
{
	return !ship_manager_is_walkable(j, i);
}

static void def_init(void)
{
	ship *tmp;
	pool_init(POOL_MAX);
	texman_init();
	//commander_init(&context_state, SERVER_IPV4/NULL)
	commander_init();

	scene_man_init();
	key_man_init();
	ship_manager_init();

	commander_send(INSTRUCTION_SCENE_SELECT, rand_range(0, SCENES_MAX));
	commander_send(INSTRUCTION_KEY_LOAD, key_defs);
	commander_send(INSTRUCTION_SHIP_LOAD, "Falcon", SHIP_PLAYER);
	commander_send(INSTRUCTION_DUDES_LOAD, SHIP_PLAYER, "Humans", 5);

	pool_usage();
}

static void def_cleanup(void)
{
	dude_unload();
	ship_manager_cleanup();
	key_man_cleanup();
	scene_man_cleanup();
	texman_cleanup();
	pool_cleanup();
}

void context_set_scene(unsigned idx)
{
	assert(idx < SCENES_MAX);
	loaded_scene = scene_defs[idx];
	scene_man_load(loaded_scene);
}

/* 
 * Public Function Definitions
 */

void context_init(void)
{
	def_init();
	astar_init(GRIDS_WIDE, GRIDS_TALL, player_ship_astar_cb);
}

void context_cleanup(void)
{
	astar_cleanup();
	def_cleanup();
}

void context_reload(void)
{
	def_cleanup();
	def_init();
}

void context_update(void)
{
	if (IsKeyPressed(KEY_F)) {
		ToggleFullscreen();
	}
	if (IsKeyPressed(KEY_R)) {
		commander_send(INSTRUCTION_RELOAD);
	}
	commander_update();
	scene_man_update();
	ship_manager_update();
	dude_select_update();
	dude_update();
}

void context_draw(void)
{
	scene_man_draw();
	ship_manager_draw();
	dude_draw();
	key_man_update();
	dude_select_draw();
	DrawFPS(0, 0);
}
