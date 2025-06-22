// draw.c
#include <draw/draw.h>
#include <map/map.h>
#include <obj/spike.h>
#include <tools/lerp/lerp.h>

static Texture2D bgTexture;
static Rectangle bgSrc, bgDst;
static Vector2 bgOrigin;

static Rectangle deadzone;
static Vector2 lookAhead;
static float lookAheadDist = 40.0f;
static float smoothSpeed = 6.0f;

void InitDrawingSystem(Texture2D bg, float screenW, float screenH) {
	bgTexture = bg;
	bgSrc = (Rectangle){ 0, 0, (float)bg.width, (float)bg.height };
	bgDst = (Rectangle){ 0, 0, screenW, screenH };
	bgOrigin = (Vector2){ 0, 0 };

	deadzone = (Rectangle){
		screenW / 2.0f - 50,
		screenH / 2.0f - 40,
		100,
		80
	};

	lookAhead = (Vector2){ 0, 0 };
}

void UpdateGameCamera(Camera2D *camera, float dt, const Player *player) {
    // Parámetros del mundo
    const float worldTopY  =   0.0f;    // Y mínima de tu mundo
    const float baseFloorY = 900.0f;    // Y del suelo base

    // Deadzone
    float dzW = GetScreenWidth()  * 0.25f;
    float dzH = GetScreenHeight() * 0.25f;
    Rectangle dz = {
        .x      = camera->target.x - dzW * 0.5f,
        .y      = camera->target.y - dzH * 0.5f,
        .width  = dzW,
        .height = dzH
    };

    // desired según walking / deadzone
    Vector2 desired = camera->target;
    bool isWalking = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT);

    if (isWalking) {
        if      (player->pos.x < dz.x)               desired.x = player->pos.x + dzW * 0.5f;
        else if (player->pos.x > dz.x + dz.width)    desired.x = player->pos.x - dzW * 0.5f;
        if      (player->pos.y < dz.y)               desired.y = player->pos.y + dzH * 0.5f;
        else if (player->pos.y > dz.y + dz.height)   desired.y = player->pos.y - dzH * 0.5f;
    } else {
        // al soltar, recentraliza solo en X
        desired.x = player->pos.x;
    }

    // suavizado
    float t_x = ((isWalking ? smoothSpeed : smoothSpeed * 0.3f) * dt);
    float t_y =  ( smoothSpeed * dt);
    if (t_x > 1.0f) t_x = 1.0f;
    if (t_y > 1.0f) t_y = 1.0f;

    camera->target.x = MyLerp(camera->target.x, desired.x, t_x);
    camera->target.y = MyLerp(camera->target.y, desired.y, t_y);

    // 🔒 clamp en Y usando +offset
    float topLimit    = worldTopY  + camera->offset.y;
    float bottomLimit = baseFloorY + camera->offset.y;

    if (camera->target.y < topLimit)    camera->target.y = topLimit;
    if (camera->target.y > bottomLimit) camera->target.y = bottomLimit;
}


void DrawGame(const Camera2D *camera, const Player *player) {
	BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexturePro(bgTexture, bgSrc, bgDst, bgOrigin, 0.0f, WHITE);
		BeginMode2D(*camera);
			DrawPlatforms();
			DrawPlayer(player);
			DrawSpikes();
		EndMode2D();
	EndDrawing();
}
