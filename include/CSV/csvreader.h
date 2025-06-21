#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CSV/csv.h>

typedef struct {
	float x, y, width, height;
} CSVRec;

typedef enum {
	TYPE_PLATFORM,
	TYPE_SPIKE
} ObjectType;

typedef struct {
	CSVRec rect;
	ObjectType type;
} GameObject;

void LoadMapFromCSV(const char* filename);

#define MAX_OBJECTS 1024