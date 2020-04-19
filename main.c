#include <stdlib.h>
#include <raylib.h>

#include "scene_manager.h"
#include "util.h"

const int screen_width = 1600;
const int screen_height = 900;

int main(void)
{
	bool launch_beetles = false;

	pool_init(100);

	InitWindow(screen_width, screen_height, "Outlands");
	SetTargetFPS(60);

	scene_man_init();
	scene_man_emplace("Paragon", 3, scenes.paragon, NULL);
	scene_man_emplace("Beetles", 200, scenes.beetle_fleet, &launch_beetles);

	scene_man_set_visible("Beetles", false);

	pool_usage();

	while (!WindowShouldClose()) {

		launch_beetles = IsKeyDown(KEY_SPACE);

		if (IsKeyPressed(KEY_A)) {
			scene_man_set_visible("Beetles", true);
		}
		if (IsKeyPressed(KEY_S)) {
			scene_man_set_visible("Beetles", false);
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
