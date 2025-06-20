#include <map/map.h>
#include <raylib.h>
#include <player/player.h>

int main() {
    InitWindow(1024, 512, "Jumping Knight");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    InitMap();

    Player player;
    InitPlayer(&player);

    Camera2D camera = { 0 };
    camera.zoom = 1.5f;
    camera.offset = (Vector2){ 512, 256 };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdatePlayer(&player, dt, platforms, numPlatforms);

        camera.target = player.pos;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                DrawMap();
                DrawPlayer(&player);
            EndMode2D();

        EndDrawing();
    }

    UnloadPlayer(&player);
    CloseWindow();
    return 0;
}
