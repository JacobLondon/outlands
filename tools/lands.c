#include <raylib.h>
#include <outlands.h>

int main(void)
{
	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Outlands");
	SetTargetFPS(DEFAULT_TARGET_FPS);
	context_init();

	while (!WindowShouldClose()) {
		context_update();
		BeginDrawing();
		context_draw();
		EndDrawing();
	}

	context_cleanup();
	CloseWindow();

	return 0;
}
