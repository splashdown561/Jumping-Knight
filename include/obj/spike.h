#ifndef SPIKE_H
#define SPIKE_H

#include <raylib.h>
#include <stdbool.h>

#include <player/player.h>

#define MAX_SPIKES 1024

typedef struct Spike
{

    Rectangle rec;
    bool isActive;

} Spike;

extern Spike spikes[MAX_SPIKES];
extern int numSpikes;

void DrawSpikes(void);

void CheckSpikeCollision(Rectangle *playerRect, Vector2 spawn);

#endif