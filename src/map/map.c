#include <map/map.h>

Platform platforms[MAX_PLATFORMS];
int numPlatforms = 0;

void InitMap(void) {
    // Ejemplo: creamos algunas plataformas manualmente
    numPlatforms = 4;

    platforms[0] = (Platform){ .rec = (Rectangle){ 100, 400, 150, 20 }, .active = true };
    platforms[1] = (Platform){ .rec = (Rectangle){ 300, 300, 100, 20 }, .active = true };
    platforms[2] = (Platform){ .rec = (Rectangle){ 500, 450, 200, 20 }, .active = true };
    platforms[3] = (Platform){ .rec = (Rectangle){ 700, 350, 120, 20 }, .active = true };

    // Si quieres agregar más, simplemente aumenta numPlatforms (hasta MAX_PLATFORMS)
}

void DrawMap(void) {
    for (int i = 0; i < numPlatforms; i++) {
        if (platforms[i].active) {
            DrawRectangleRec(platforms[i].rec, DARKGRAY);
        }
    }
}
