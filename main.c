#include <stdlib.h>
#include <raylib.h>

#include "scene_manager.h"
#include "globals.h"
#include "obj_key.h"
#include "util.h"

char *default_scene[] = {
	"Paragon", "Beetles", "Explosion", NULL
};

static void laser_cb_0(object_key *self, so *object)
{
	// wait until the key press, then go!
	if (IsKeyPressed(KEY_J)) {
		so_set_pos(object, GetScreenWidth(), GetScreenHeight() / 2);
		object_key_set_key(self, true);
	}
}

static void laser_cb_1(object_key *self, so *object)
{
	const Vector2 p = so_get_pos(object);
	const int x = GetMouseX();
	const int y = GetMouseY();

	// move p towards mouse
	if (p.x > x) {
		so_set_pos(object, p.x - 10, p.y);
	}
	else if (p.x < x) {
		so_set_pos(object, p.x + 10, p.y);
	}
	if (p.y > y) {
		so_set_pos(object, p.x, p.y - 10);
	}
	else if (p.y < y) {
		so_set_pos(object, p.x, p.y + 10);
	}

	// once to the right of, close enough, next state
	if (p.x < x) {
		object_key_set_key(self, true);
	}
}

static void laser_cb_2(object_key *self, so *object)
{
	so_set_pos(object, GetMouseX(), GetMouseY());
	// just animate and stop near the end
	if (object_key_get_oof(self) > 0.9) {
		object_key_set_key(self, true);
	}
}

int main(void)
{
	pool_init(100);

	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Outlands");
	SetTargetFPS(DEFAULT_TARGET_FPS);

	scene_man_init();
	scene_man_load(default_scene);
	scene_man_tie_visibility("Beetles", &beetles_visible);

	object_key *laser_shot = object_key_new();
		anim *tmp = anim_new("assets/laaazaa.png", 1, 1);
		so *tmpso = so_new(tmp);
	object_key_add(laser_shot, tmpso, laser_cb_0, NULL); // spawn on j press
	object_key_add(laser_shot, tmpso, laser_cb_1, NULL); // move to mouse
		tmp = anim_new("assets/explosion3.png", 4, 4);
		tmpso = so_new(tmp);
	object_key_add(laser_shot, tmpso, laser_cb_2, NULL); // explode

	object_key *mycopy = object_key_copy(laser_shot);

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

			if (mycopy) {
				object_key_update(mycopy);
			}
			else {
				mycopy = object_key_copy(laser_shot);
			}
		}
		EndDrawing();
	}

	scene_man_cleanup();
	pool_cleanup();

	CloseWindow();

	return 0;
}
