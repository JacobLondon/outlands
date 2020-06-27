#include <outlands.h>

int main(void)
{
	Image icon;
	InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Outlands");
	icon = LoadImage("assets/beetle-sml.png");
	SetWindowIcon(icon);
	SetTargetFPS(DEFAULT_TARGET_FPS);
	context_init();

	while (!WindowShouldClose()) {
		context_update();
		BeginDrawing();
		context_draw();
		EndDrawing();
	}

	context_cleanup();
	UnloadImage(icon);
	CloseWindow();

	return 0;
}
