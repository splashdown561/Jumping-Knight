#include <map/map.h>
#include <raylib.h>
#include <player/player.h>
#include <obj/spike.h>
#include <csv/csvreader.h>
#include <tools/lerp/lerp.h>

extern Vector2 playerSpawn;  // ✅ Esto es lo que falta
extern Texture2D texPlatform;

int w = 640;
int h = 480;

int main() {
    InitWindow(w, h, "Jumping Knight");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    LoadMapFromCSV("assets/map1.csv");

    Player player;
    InitPlayer(&player, &playerSpawn);  // Usa el spawn cargado

    Texture2D bg = LoadTexture("assets/bg.png");

    Camera2D camera = { 0 };
    texPlatform = LoadTexture("assets/stone.png");
    camera.zoom = 1.5f;
    camera.offset = (Vector2){ w/2, h/2 };
    Rectangle playerRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };

    // Zona muerta: un rectángulo en el centro
    float deadW = 100, deadH = 80;
    Rectangle deadzone = {
        w/2.0f - deadW/2.0f,
        h/2.0f - deadH/2.0f,
        deadW, deadH
    };

    // Parámetros de cámara
    float smoothSpeed   = 6.0f;    // cuanto más alto, menos retraso
    float lookAheadDist = 40.0f;   // píxeles adelante
    Vector2 lookAhead   = {0,0};   // estado persistente

    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdatePlayer(&player, dt, platforms, numPlatforms);
        camera.target = player.pos;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();

            // Rectángulo fuente (toda la textura original)
            Rectangle src = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
            // Rectángulo destino (pantalla completa)
            Rectangle dst = { 0.0f, 0.0f, (float)screenW, (float)screenH };
            // Origen de rotación/escala
            Vector2 origin = { 0.0f, 0.0f };

            DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);

            // 1) calcula la posición deseada con look‑ahead
            Vector2 playerScreenPos = GetWorldToScreen2D(player.pos, camera);
            Vector2 desired = player.pos;

// Si el jugador se sale de la deadzone horizontal:
            if (playerScreenPos.x < deadzone.x) {
                desired.x -= (deadzone.x - playerScreenPos.x);
            } else if (playerScreenPos.x > deadzone.x + deadzone.width) {
                desired.x += (playerScreenPos.x - (deadzone.x + deadzone.width));
            }

    // Igual para eje Y:
            if (playerScreenPos.y < deadzone.y) {
                desired.y -= (deadzone.y - playerScreenPos.y);
            } else if (playerScreenPos.y > deadzone.y + deadzone.height) {
                desired.y += (playerScreenPos.y - (deadzone.y + deadzone.height));
            }

    // Look‑ahead en X según velocidad:
            float dir = (IsKeyDown(KEY_RIGHT) ? +1 : IsKeyDown(KEY_LEFT) ? -1 : 0);
            Vector2 targetLook = (Vector2){ lookAheadDist * dir, 0 };
            lookAhead.x = MyLerp(lookAhead.x, targetLook.x, dt * smoothSpeed);
            lookAhead.y = MyLerp(lookAhead.y, 0, dt * smoothSpeed);

    // Aplicar look‑ahead al objetivo
            desired.x += lookAhead.x;
            desired.y += lookAhead.y;

    // 2) suavizado de la cámara
            camera.target.x = MyLerp(camera.target.x, desired.x, dt * smoothSpeed);
            camera.target.y = MyLerp(camera.target.y, desired.y, dt * smoothSpeed);


            BeginMode2D(camera);
                if (IsKeyPressed(KEY_R))
                {
                    player.pos = playerSpawn;
                    LoadMapFromCSV("assets/map1.csv");
                } 
                DrawPlatforms();
                DrawPlayer(&player);
                DrawSpikes();
            EndMode2D();
        EndDrawing();
    }

    UnloadPlayer(&player);
    UnloadTexture(texPlatform);
    CloseWindow();
    return 0;
}
