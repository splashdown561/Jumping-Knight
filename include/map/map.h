#ifndef MAP_H
#define MAP_H

#include <raylib.h>

#define MAX_PLATFORMS 10

typedef struct {
    Rectangle rec;
    bool active;
} Platform;

extern Platform platforms[MAX_PLATFORMS];
extern int numPlatforms;

void InitMap();
void DrawMap();

#endif // MAP_H
