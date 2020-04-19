#include <stdlib.h>
#include <raylib.h>

#include "scene_manager.h"
#include "globals.h"
#include "util.h"

const int screen_width = 1600;
const int screen_height = 900;

char *default_scene[] = {
	"Paragon", "Beetles", NULL
};

int main(void)
{
	pool_init(100);

	InitWindow(screen_width, screen_height, "Outlands");
	SetTargetFPS(60);

	scene_man_init();
	scene_man_load(default_scene);
	scene_man_tie_visibility("Beetles", &beetles_visible);

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
		}
		EndDrawing();
	}

	scene_man_cleanup();
	pool_cleanup();

	CloseWindow();

	return 0;
}
