#include <obj/spike.h>
#include <stdio.h>

Spike spikes[MAX_SPIKES];
int numSpikes = 0;

void DrawSpikes(void) {
	for (int i = 0; i < numSpikes; i++) {
		if (spikes[i].isActive) {
			DrawRectangleRec(spikes[i].rec, RED); // o usa una textura
		}
	}
}

void CheckSpikeCollision(Rectangle *playerRect, Vector2 spawn) {
	for (int i = 0; i < numSpikes; i++) {
		if (spikes[i].isActive && CheckCollisionRecs(*playerRect, spikes[i].rec)) {
			printf("💀 Colisión con spike #%d. Reiniciando jugador...\n", i);
			playerRect->x = spawn.x;
			playerRect->y = spawn.y;
			break;
		}
	}
}
