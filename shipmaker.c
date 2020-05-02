#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "ship_tile.h"
#include "ship.h"
#include "texture_manager.h"
#include "globals.h"

#define TEXTURES_MAX 128
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

enum mode {
	NO_MODE,
	EDIT_MODE,
	NEW_MODE
};

int ids[GRIDS_TALL][GRIDS_WIDE] = { 0 };
char *ship_png = NULL;
char *ship_rpg = NULL;
enum mode editor_mode = NO_MODE;

static void save_rpg(void);

int main(int argc, char **argv)
{
	char *arg;
	int i, j;
	int ci, cj;
	static char number[8];
	int increment = 1;
	Texture2D *textures[TEXTURES_MAX] = { 0 };
	Texture2D *scene[3];

	/* 
	 * Arg Parsing
	 */

	if (arg = arg_get(argc, argv, "open")) {
		ship_png = arg;
	}
	if ((editor_mode == NO_MODE) && (arg = arg_get(argc, argv, "new"))) {
		ship_rpg = arg;
		editor_mode = NEW_MODE;
	}
	if ((editor_mode == NO_MODE) && (arg = arg_get(argc, argv, "edit"))) {
		ship_rpg = arg;
		editor_mode = EDIT_MODE;
	}
	if (!ship_png || !ship_rpg || editor_mode == NO_MODE) {
		fprintf(stderr, "Usage: maker.exe open {assets/<ship_png>.png} [new|edit] {rpgs/<ship>.rpg}\n");
		exit(1);
	}

	if (editor_mode == EDIT_MODE) {
		ship_load_rpg(ship_rpg, (int *)ids, GRIDS_WIDE, GRIDS_TALL);
	}

	/* 
	 * Editor
	 */

	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Ship Maker");
	SetTargetFPS(60);

	texman_init();

	scene[0] = texman_load("assets/star 3.png");
	scene[1] = texman_load("assets/Skyrillis.png");
	scene[2] = texman_load(ship_png);
	for (i = 1; i < ship_tile_get_count() && i < TEXTURES_MAX; i++) {
		textures[i] = texman_load(ship_tile_get_png(i));
	}

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_ENTER)) {
			save_rpg();
		}

		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			increment = 5;
		}

		ci = GetMouseY() / GRID_PIX_HEIGHT;
		cj = GetMouseX() / GRID_PIX_WIDTH;

		if (IsMouseButtonPressed(0)) {
			if (ids[ci][cj] + increment < ship_tile_get_count()) {
				ids[ci][cj] += increment;
			}
		}
		else if (IsMouseButtonPressed(1)) {
			if (ids[ci][cj] - increment >= 0) {
				ids[ci][cj] -= increment;
			}
		}

		if (IsKeyUp(KEY_LEFT_SHIFT)) {
			increment = 1;
		}

		BeginDrawing();

		for (i = 0; i < SIZE(scene); i++) {
			DrawTexture(*scene[i], 0, 0, WHITE);
		}
		
		// draw that grid
		for (i = 0; i < GRIDS_TALL; i++) {
			for (j = 0; j < GRIDS_WIDE; j++) {
				if (ids[i][j] > 0) {
					texman_test_draw(textures[ids[i][j]], j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT);
					memset(number, 0, sizeof(number));
					sprintf(number, "%d", ids[i][j]);
					number[sizeof(number) - 1] = 0;
					DrawText(number, j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT, 16, BLACK);
					DrawText(number, j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT, 12, WHITE);
				}

			}
		}

		EndDrawing();
	}

	texman_cleanup();
	CloseWindow();

	return 0;
}

static void save_rpg(void)
{
	int i, j;
	char buf[128] = { 0 };
	FILE *f;
	
	// reset the file
	f = fopen(ship_rpg, "w");
	if (!f) {
		fprintf(stderr, "Cannot open file %s", ship_rpg);
		exit(2);
	}
	for (i = 0; i < GRIDS_TALL; i++) {
		for (j = 0; j < GRIDS_WIDE; j++) {
			if (ids[i][j] != 0) {
				memset(buf, 0, sizeof(buf));
				fprintf(f, "%d %d %d\n", i, j, ids[i][j]);
			}
		}
	}

	fclose(f);
}
