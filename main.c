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
	so_set_pos(object, GetScreenWidth(), GetScreenHeight() / 2);
	// wait until the key press, then go!
	if (IsKeyPressed(KEY_J)) {
		object_key_set_key(self, true);
	}
}

static void laser_cb_1(object_key *self, so *object)
{
	const Vector2 p = so_get_pos(object);
	const int x = GetMouseX();
	const int w = anim_get_width(so_get_anim(object));

	// once to the right of, close enough, next state
	if (p.x < x) {
		object_key_set_key(self, true);
	}
}

static void laser_cb_2(object_key *self, so *object)
{
	const int w = anim_get_width(so_get_anim(object));
	const int h = anim_get_height(so_get_anim(object));
	so_set_pos(object, GetMouseX() - w / 2, GetMouseY() - h / 2);
	// just animate and stop near the end
	if (object_key_get_frame(self) > 15) {
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
		anim *tmp = anim_new("assets/balls.png", 1, 4);
		so *tmpso = so_new(tmp);
		so_set_pos(tmpso, GetScreenWidth(), GetScreenHeight() / 2);
		so_newmov(tmpso, so_cb_left, 30, NULL);
		so_newmov(tmpso, so_cb_trk_vrt, 3, NULL);
	object_key_add(laser_shot, tmpso, laser_cb_0, NULL); // spawn on j press
	object_key_add_rate(laser_shot, tmpso, laser_cb_1, NULL, 8); // move to mouse
		tmp = anim_new("assets/explosion3.png", 4, 4);
		tmpso = so_new(tmp);
	object_key_add_rate(laser_shot, tmpso, laser_cb_2, NULL, 16); // explode

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
			if (object_key_update(mycopy)) {
				mycopy = object_key_copy(laser_shot);
			}
			DrawFPS(0, 0);
		}
		EndDrawing();
	}

	scene_man_cleanup();
	pool_cleanup();

	CloseWindow();

	return 0;
}
