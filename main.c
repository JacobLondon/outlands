#include <stdlib.h>
#include <raylib.h>

#include "man.h"
#include "imger.h"
#include "util.h"

const int screen_width = 1600;
const int screen_height = 900;

int main(void)
{
	int i;
	so *tmp, *template;
	man *manager;
	imger *im;
	pool_init(100);

	InitWindow(screen_width, screen_height, "Outlands");
	SetTargetFPS(60);

	im = imger_new();
	manager = man_new();

	tmp = so_new(imger_load(im, "assets/star 3.png"));
	so_set_pos(tmp, 0, 0);
	man_scene_load(manager, tmp);

	tmp = so_new(imger_load(im, "assets/Lyra Paragon.png"));
	so_newmov(tmp, so_move_loop_left, 0.1, NULL);
	man_scene_load(manager, tmp);

	template = so_new(imger_load(im, "assets/beetle-sml.png"));
	so_newmov(template, so_move_loop_up, 10, NULL);
	so_newmov(template, so_move_bob_hrz, 10, NULL);
	man_scene_load(manager, template);
	for (i = 0; i < 120; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.01 + 0.1);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		man_scene_load(manager, tmp);
	}
	pool_usage();

	while (!WindowShouldClose()) {

		man_scene_update(manager);

		BeginDrawing();

		ClearBackground(BLACK);
		man_scene_draw(manager);

		EndDrawing();
	}

	man_del(manager);
	imger_del(im);
	pool_cleanup();
	CloseWindow();

	return 0;
}
