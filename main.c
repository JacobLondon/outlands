#include <stdlib.h>
#include <raylib.h>

#include "scene_manager.h"
#include "globals.h"
#include "util.h"
#include "texture_manager.h"
#include "key_manager.h"

#define SCENES_MAX 3

static char *default_scenes[SCENES_MAX][] = {
	{ "Heavy Nebula 2", "Gluurus", "Beetles", "FTL-Light", NULL },
	{ "Star1", "Skyrillis", "Asteroids", "FTL-Light", NULL },
	{ "Space3", "Reitis", "Executives", "FTL-Light", NULL },
};

static char *key_objects[] = {
	"Missile", NULL
};

int main(void)
{
	pool_init(100);
	texman_init();

	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Outlands");
	SetTargetFPS(DEFAULT_TARGET_FPS);

	scene_man_init();
	scene_man_load(default_scenes[rand_range(0, SCENES_MAX)]);

	key_man_init();
	key_man_load(key_objects);

	pool_usage();

	while (!WindowShouldClose()) {

		beetles_launch = !IsKeyDown(KEY_SPACE);

		if (IsKeyPressed(KEY_A)) {
			beetles_visible = true;
		}
		if (IsKeyPressed(KEY_S)) {
			beetles_visible = false;
		}
		scene_man_update();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			scene_man_draw();
			key_man_update();
			DrawFPS(0, 0);
		}
		EndDrawing();
	}

	key_man_cleanup();
	scene_man_cleanup();
	texman_cleanup();
	pool_cleanup();

	CloseWindow();

	return 0;
}
