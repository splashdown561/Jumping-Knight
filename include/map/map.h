#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_PLATFORMS 256

typedef struct Platform {
	bool active;
	Rectangle rec;
} Platform;

extern Platform platforms[MAX_PLATFORMS];
extern int numPlatforms;

extern Texture2D texPlatform;

void DrawPlatforms(void);

#endif
