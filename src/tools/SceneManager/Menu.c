#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <tools/SceneManager/Scene.h>

extern Scene GameScene; // ✅ Esta línea es necesaria

static void MenuStart() {}

static void MenuUpdate(float dt) {
	if (GuiButton((Rectangle){ 270, 180, 100, 30 }, "JUGAR")) {
		SetScene(&GameScene);  // Esto ya no dará error
	}
}

static void MenuDraw() {
	BeginDrawing();
		ClearBackground(DARKGRAY);
		Font coolvetica = LoadFont("assets/fonts/coolveticarg.ttf");
		GuiSetFont(coolvetica);
		int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE); // Guardar tamaño original
		GuiSetStyle(DEFAULT, TEXT_SIZE, 30); 
		GuiLabel((Rectangle){ 240, 120, 400, 30 }, "Jumping Knight");
		GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
		GuiButton((Rectangle){ 270, 180, 100, 30 }, "JUGAR");
	EndDrawing();
}

static void MenuUnload() {}

Scene MenuScene = {
	.Start  = MenuStart,
	.Update = MenuUpdate,
	.Draw   = MenuDraw,
	.Unload = MenuUnload
};
