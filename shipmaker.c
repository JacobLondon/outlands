#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "ship_tile.h"
#include "texture_manager.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800
#define GRID_WIDTH 40
#define GRID_HEIGHT 20
#define GRID_PIX_WIDTH (SCREEN_WIDTH / GRID_WIDTH)
#define GRID_PIX_HEIGHT (SCREEN_HEIGHT / GRID_HEIGHT)
#define TEXTURES_MAX 128
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

enum mode {
	NO_MODE,
	EDIT_MODE,
	NEW_MODE
};

int tiles[GRID_HEIGHT][GRID_WIDTH] = { 0 };
char *ship_path = NULL;
char *result = NULL;
enum mode editor_mode = NO_MODE;

static void save(void);
static void load(void);

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
		ship_path = arg;
	}
	if ((editor_mode == NO_MODE) && (arg = arg_get(argc, argv, "new"))) {
		result = arg;
		editor_mode = NEW_MODE;
	}
	if ((editor_mode == NO_MODE) && (arg = arg_get(argc, argv, "edit"))) {
		result = arg;
		editor_mode = EDIT_MODE;
	}
	if (!ship_path || !result || editor_mode == NO_MODE) {
		fprintf(stderr, "Usage: maker.exe open {assets/<ship_path>.png} [new|edit] {rpgs/<ship>.rpg}\n");
		exit(1);
	}

	if (editor_mode == EDIT_MODE) {
		load();
	}

	/* 
	 * Editor
	 */

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ship Maker");
	SetTargetFPS(60);

	texman_init();

	scene[0] = texman_load("assets/star 3.png");
	scene[1] = texman_load("assets/Skyrillis.png");
	scene[2] = texman_load(ship_path);
	for (i = 1; i < ship_tile_get_count() && i < TEXTURES_MAX; i++) {
		textures[i] = texman_load(ship_tile_get_png(i));
	}

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_ENTER)) {
			save();
		}

		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			increment = 5;
		}

		ci = GetMouseY() / GRID_PIX_HEIGHT;
		cj = GetMouseX() / GRID_PIX_WIDTH;

		if (IsMouseButtonPressed(0)) {
			if (tiles[ci][cj] + increment < ship_tile_get_count()) {
				tiles[ci][cj] += increment;
			}
		}
		else if (IsMouseButtonPressed(1)) {
			if (tiles[ci][cj] - increment >= 0) {
				tiles[ci][cj] -= increment;
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
		for (i = 0; i < GRID_HEIGHT; i++) {
			for (j = 0; j < GRID_WIDTH; j++) {
				if (tiles[i][j] > 0) {
					/*DrawRectangle(
						j * GRID_PIX_WIDTH,
						i * GRID_PIX_HEIGHT,
						GRID_PIX_WIDTH,
						GRID_PIX_HEIGHT,
						(Color){255, 0, 0, 50}
					);*/
					texman_test_draw(textures[tiles[i][j]], j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT);
					memset(number, 0, sizeof(number));
					sprintf(number, "%d", tiles[i][j]);
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

static void save(void)
{
	int i, j;
	char buf[128] = { 0 };
	FILE *f;
	
	// reset the file
	f = fopen(result, "w");
	if (!f) {
		fprintf(stderr, "Cannot open file %s", result);
		exit(2);
	}
	for (i = 0; i < GRID_HEIGHT; i++) {
		for (j = 0; j < GRID_WIDTH; j++) {
			if (tiles[i][j] != 0) {
				memset(buf, 0, sizeof(buf));
				fprintf(f, "%d %d %d\n", i, j, tiles[i][j]);
			}
		}
	}

	fclose(f);
}

static void load(void)
{
	char *rpg;
	char *p;
	int end;
	size_t size;
	int i, j, id;

	memset(tiles, 0, sizeof(tiles));
	p = rpg = file_read(result, &size);

	while (p - rpg < size) {
		end = strcspn(p, "\n");
		if (sscanf(p, "%d%d%d", &i, &j, &id) != 3) {
			fprintf(stderr, "Failed reading line at %s\n", p);
			exit(3);
		}
		tiles[i][j] = id;

		// bypass the \n to get to next line so strcspn != 0 forever
		p += end + 1;
	}
	free(rpg);
}
