#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CSV/csv.h>

typedef struct {
	float x, y, width, height;
} Rectangle;

typedef enum {
	TYPE_PLATFORM,
	TYPE_SPIKE
} ObjectType;

typedef struct {
	Rectangle rect;
	ObjectType type;
} GameObject;

#define MAX_OBJECTS 1024