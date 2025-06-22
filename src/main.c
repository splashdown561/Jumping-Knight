#include <raylib.h>
#include <tools/SceneManager/Scene.h>

extern Scene MenuScene;

int main() {
	InitWindow(640, 480, "Jumping Knight");
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	SetScene(&MenuScene); // <- esto carga primero el menú

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
		UpdateScene(dt);
		DrawScene();
	}

	UnloadCurrentScene();
	CloseWindow();
	return 0;
}
