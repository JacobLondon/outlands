#include <stdlib.h>
#include <raylib.h>

#include "scene.h"
#include "imger.h"
#include "util.h"

const int screen_width = 1600;
const int screen_height = 900;

int main(void)
{
	int i;
	so *tmp, *template;
	scene *sc;
	imger *im;
	pool_init(100);

	InitWindow(screen_width, screen_height, "Outlands");
	SetTargetFPS(60);

	im = imger_new();
	sc = scene_new("Default", NULL, NULL);

	tmp = so_new(imger_load(im, "assets/star 3.png"));
	so_set_pos(tmp, 0, 0);
	scene_load_so(sc, tmp);

	tmp = so_new(imger_load(im, "assets/Lyra Paragon.png"));
	so_newmov(tmp, so_move_loop_left, 0.1, NULL);
	scene_load_so(sc, tmp);

	template = so_new(imger_load(im, "assets/beetle-sml.png"));
	so_newmov(template, so_move_loop_up, 10, NULL);
	so_newmov(template, so_move_bob_hrz, 10, NULL);
	scene_load_so(sc, template);
	for (i = 0; i < 50; i++) {
		tmp = so_copy(template);
		so_set_bobrate(tmp, rand_uniform() * 0.05);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		scene_load_so(sc, tmp);
	}
	pool_usage();

	while (!WindowShouldClose()) {

		scene_update(sc);

		BeginDrawing();

		ClearBackground(BLACK);
		scene_draw(sc);

		EndDrawing();
	}

	scene_del(sc);
	imger_del(im);
	pool_cleanup();
	CloseWindow();

	return 0;
}
