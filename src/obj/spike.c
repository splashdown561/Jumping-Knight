#include <obj/spike.h>
#include <CSV/csvreader.h>
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

void CheckSpikeCollision(Rectangle *playerRect) {
    for (int i = 0; i < numSpikes; i++) {
        // Si el jugador colisiona con un pincho
        if (CheckCollisionRecs(*playerRect, spikes[i].rec)) {
            printf("⚠️ Colisión con pincho en: (%.1f, %.1f)\n", spikes[i].rec.x, spikes[i].rec.y);
            
            // Reiniciar la posición del jugador al spawn
            playerRect->x = playerSpawn.x;
            playerRect->y = playerSpawn.y;
            printf("👾 Jugador reiniciado en: (%.1f, %.1f)\n", playerSpawn.x, playerSpawn.y);
        }
    }
}

