// scene/game_scene.c
#include <raylib.h>
#include <player/player.h>
#include <map/map.h>
#include <obj/spike.h>
#include <csv/csvreader.h>
#include <tools/lerp/lerp.h>
#include <draw/draw.h>
#include <tools/SceneManager/Scene.h>
#include <globals.h>

static Player player;
static Camera2D camera;
static Texture2D bg;

Texture2D texPlatform;
Vector2 playerSpawn = {0};

static void GameStart() {
	LoadMapFromCSV("assets/map1.csv");
	bg = LoadTexture("assets/bg.png");
	texPlatform = LoadTexture("assets/stone.png");
	InitPlayer(&player, &playerSpawn);

	camera.zoom = 1.5f;
	camera.offset = (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2 };
	InitDrawingSystem(bg, GetScreenWidth(), GetScreenHeight());
}

static void GameUpdate(float dt) {
	UpdatePlayer(&player, dt, platforms, numPlatforms);
	UpdateGameCamera(&camera, dt, &player);

	if (IsKeyPressed(KEY_R)) {
		player.pos = playerSpawn;
		LoadMapFromCSV("assets/map1.csv");
	}
}

static void GameDraw() {
	DrawGame(&camera, &player);
}

static void GameUnload() {
	UnloadPlayer(&player);
	UnloadTexture(bg);
	UnloadTexture(texPlatform);
}

Scene GameScene = {
	.Start  = GameStart,
	.Update = GameUpdate,
	.Draw   = GameDraw,
	.Unload = GameUnload
};
