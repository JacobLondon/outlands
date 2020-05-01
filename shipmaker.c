#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "util.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800
#define GRID_WIDTH 40
#define GRID_HEIGHT 20
#define GRID_PIX_WIDTH (SCREEN_WIDTH / GRID_WIDTH)
#define GRID_PIX_HEIGHT (SCREEN_HEIGHT / GRID_HEIGHT)
#define SIZE(array) (sizeof(array) / sizeof(array[0]))

int tiles[GRID_HEIGHT][GRID_WIDTH] = { 0 };
char *ship_path = NULL;
char *result = NULL;

static void save(void);

int main(int argc, char **argv)
{
	int i, j;
	Texture2D textures[3];
	static char number[8];
	int increment = 1;

	if (argc != 3) {
		fprintf(stderr, "Usage: maker.exe {rpgs/<ship>.rpg} {assets/<ship_path>.png}\n");
		exit(1);
	}
	result = argv[1];
	ship_path = argv[2];

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ship Maker");
	SetTargetFPS(60);
	
	textures[0] = LoadTexture("assets/star 3.png");
	textures[1] = LoadTexture("assets/Skyrillis.png");
	textures[2] = LoadTexture(ship_path);

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_ENTER)) {
			save();
		}

		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			increment = 5;
		}

		if (IsMouseButtonPressed(0)) {
			tiles[GetMouseY() / GRID_PIX_HEIGHT][GetMouseX() / GRID_PIX_WIDTH] += increment;
		}
		else if (IsMouseButtonPressed(1)) {
			if (tiles[GetMouseY() / GRID_PIX_HEIGHT][GetMouseX() / GRID_PIX_WIDTH] - increment >= 0) {
				tiles[GetMouseY() / GRID_PIX_HEIGHT][GetMouseX() / GRID_PIX_WIDTH] -= increment;
			}
		}

		if (IsKeyUp(KEY_LEFT_SHIFT)) {
			increment = 1;
		}

		BeginDrawing();

		for (i = 0; i < SIZE(textures); i++) {
			DrawTexture(textures[i], 0, 0, WHITE);
		}
		
		// draw that grid
		for (i = 0; i < GRID_HEIGHT; i++) {
			for (j = 0; j < GRID_WIDTH; j++) {
				if (tiles[i][j] > 0) {
					memset(number, 0, sizeof(number));
					DrawRectangle(
						j * GRID_PIX_WIDTH,
						i * GRID_PIX_HEIGHT,
						GRID_PIX_WIDTH,
						GRID_PIX_HEIGHT,
						(Color){255, 0, 0, 50}
					);
					sprintf(number, "%d", tiles[i][j]);
					number[sizeof(number) - 1] = 0;
					DrawText(number, j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT, 16, BLACK);
					DrawText(number, j * GRID_PIX_WIDTH, i * GRID_PIX_HEIGHT, 12, WHITE);
				}

			}
		}

		EndDrawing();
	}

	for (i = 0; i < SIZE(tiles); i++) {
		UnloadTexture(textures[i]);
	}

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
	fprintf(f, "");
	fclose(f);

	// write the data
	f = fopen(result, "ab");
	if (!f) {
		fprintf(stderr, "Cannot open file %s", result);
		exit(3);
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
