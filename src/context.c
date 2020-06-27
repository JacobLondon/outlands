#include <stddef.h>
#include "scene_man.h"
#include "texture_man.h"
#include "key_man.h"
#include "astar.h"
#include "ship_man.h"
#include "dude.h"
#include "globals.h"
#include "util.h"
#include "context.h"

// copied or unique key objects
#define KEYS_MAX 32
#define SCENES_MAX 5
#define SHIPS_MAX 32
#define POOL_MAX 60 /* kilobytes for visuals management, also the map size of nodes for A* is like 150kB */

static char *key_defs[] = {
	"Missile", NULL
};

// TODO: ship manager utility instead
/*static char *ship_defs[] = {
	"Falcon", NULL
};*/

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
	return !ship_man_is_walkable(j, i);
}

static void def_init(void)
{
	pool_init(POOL_MAX);
	texture_man_init();

	scene_man_init();
	key_man_init();
	ship_man_init();

	scene_man_load_rand();
	key_man_load(key_defs);
	ship_man_load("Falcon", SHIP_PLAYER);
	dude_load(5, "Humans", ship_man_get(SHIP_PLAYER));

	pool_usage();
}

static void def_cleanup(void)
{
	dude_unload();
	ship_man_cleanup();
	key_man_cleanup();
	scene_man_cleanup();
	texture_man_cleanup();
	pool_cleanup();
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
		context_reload();
	}
	scene_man_update();
	ship_man_update();
	dude_select_update();
	dude_update();
}

void context_draw(void)
{
	scene_man_draw();
	ship_man_draw();
	dude_draw();
	key_man_update();
	dude_select_draw();
	DrawFPS(0, 0);
}
