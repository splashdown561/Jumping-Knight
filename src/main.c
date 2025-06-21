#include <map/map.h>
#include <raylib.h>
#include <player/player.h>

#include <obj/spike.h>
#include <csv/csvreader.h>

int main() {

    Rectangle p = { 0 };

    InitWindow(1024, 512, "Jumping Knight");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    LoadMapFromCSV("assets/map1.csv");

    Player player;
    InitPlayer(&player);

    Camera2D camera = { 0 };
    camera.zoom = 1.5f;
    camera.offset = (Vector2){ 512, 256 };
    Rectangle playerRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };

    Vector2 playerSpawn = { 0 };  // Inicializado a (0,0) por defecto

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdatePlayer(&player, dt, platforms, numPlatforms);

        camera.target = player.pos;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                DrawPlatforms();
                DrawPlayer(&player);
                DrawSpikes();
                CheckSpikeCollision(&playerRect, playerSpawn);
            EndMode2D();

        EndDrawing();
    }

    UnloadPlayer(&player);
    CloseWindow();
    return 0;
}
