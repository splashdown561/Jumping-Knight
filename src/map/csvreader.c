#include <csv/csvreader.h>
#include <map/map.h>      // platforms[], numPlatforms
#include <obj/spike.h>    // spikes[], numSpikes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 4096

// Ya definido correctamente como global
Vector2 playerSpawn = { 0 };

void LoadMapFromCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo CSV");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;

    // Leer primera línea (puede ser spawn)
    if (fgets(line, sizeof(line), file)) {
        lineNumber++;
        line[strcspn(line, "\r\n")] = '\0'; // quitar salto de línea
        printf("📋 [L%d] Línea leída: '%s'\n", lineNumber, line);  // Añadir para depuración

        // Verificar que se está detectando correctamente "spawn;"
        if (strncmp(line, "spawn;", 6) == 0) {
            char *copy = strdup(line);
            strtok(copy, ";"); // "spawn"
            char *xStr = strtok(NULL, ";");
            char *yStr = strtok(NULL, ";");

            if (xStr && yStr) {
                playerSpawn.x = strtof(xStr, NULL);
                playerSpawn.y = strtof(yStr, NULL);
                printf("📍 [Spawn] Coordenadas cargadas: (%.1f, %.1f)\n", playerSpawn.x, playerSpawn.y);
            } else {
                printf("⚠️  [L%d] Formato de spawn inválido, usando (0,0)\n", lineNumber);
            }
            free(copy);
        } else {
            printf("⚠️  [L%d] No se detectó línea de spawn, usando (0,0)\n", lineNumber);
            // Si no es spawn, asumimos que fue encabezado y seguimos
        }
    }

    // Resetear estructuras
    numPlatforms = 0;
    numSpikes = 0;

    // Leer líneas del mapa
    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        char *copy = strdup(line);
        if (!copy) {
            printf("🛑 [L%d] Error de memoria\n", lineNumber);
            continue;
        }

        // Tokenizar
        char *token = strtok(copy, ";");
        if (!token) { free(copy); continue; }
        float x = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (!token) { free(copy); continue; }
        float y = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (!token) { free(copy); continue; }
        float w = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (!token) { free(copy); continue; }
        float h = strtof(token, NULL);

        token = strtok(NULL, ";");
        if (!token) { free(copy); continue; }

        char type[32];
        sscanf(token, "%31s", type);

        if (w <= 0 || h <= 0) {
            printf("⚠️  [L%d] Dimensiones inválidas w=%.1f h=%.1f, omitiendo\n", lineNumber, w, h);
            free(copy);
            continue;
        }

        // Crear objetos
        if (strcmp(type, "platform") == 0) {
            if (numPlatforms < MAX_PLATFORMS) {
                platforms[numPlatforms].rec = (Rectangle){ x, y, w, h };
                platforms[numPlatforms].active = true;
                numPlatforms++;
            } else {
                printf("⚠️  [L%d] MAX_PLATFORMS alcanzado\n", lineNumber);
            }
        }
        else if (strcmp(type, "spike") == 0) {
            if (numSpikes < MAX_SPIKES) {
                spikes[numSpikes].rec = (Rectangle){ x, y, w, h };
                spikes[numSpikes].isActive = true;
                numSpikes++;
            } else {
                printf("⚠️  [L%d] MAX_SPIKES alcanzado\n", lineNumber);
            }
        }
        else {
            printf("❓ [L%d] Tipo desconocido '%s'\n", lineNumber, type);
        }

        free(copy);
    }

    fclose(file);
    printf("✅ Carga completa: %d plataformas, %d pinchos, spawn=(%.1f, %.1f)\n",
           numPlatforms, numSpikes, playerSpawn.x, playerSpawn.y);
}
