// map.c
#include <map/map.h>
#include <raylib.h>
#include <math.h>
#include <globals.h>

Platform platforms[MAX_PLATFORMS];
int numPlatforms = 0;

void DrawPlatforms(void) {
    for (int i = 0; i < numPlatforms; i++) {
        if (!platforms[i].active) continue;
        Rectangle rec = platforms[i].rec;
        float xEnd = rec.x + rec.width;
        float yEnd = rec.y + rec.height;

        // Tile map drawing: repetir textura en la plataforma
        for (float y = rec.y; y < yEnd; y += texPlatform.height) {
            for (float x = rec.x; x < xEnd; x += texPlatform.width) {
                // Tamaño del tile (manejar bordes)
                float tileW = fminf(texPlatform.width, xEnd - x);
                float tileH = fminf(texPlatform.height, yEnd - y);

                Rectangle src = { 0, 0, tileW, tileH };
                Rectangle dst = { x, y, tileW, tileH };

                DrawTexturePro(texPlatform, src, dst, (Vector2){0,0}, 0.0f, WHITE);
            }
        }
    }
}
