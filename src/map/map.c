// map.c
#include <map/map.h>
#include <raylib.h>

Platform platforms[MAX_PLATFORMS];
int numPlatforms = 0;

void DrawPlatforms(void) {
	for (int i = 0; i < numPlatforms; i++) {
		if (!platforms[i].active) continue;
		DrawRectangleRec(platforms[i].rec, DARKGRAY);
	}
}
