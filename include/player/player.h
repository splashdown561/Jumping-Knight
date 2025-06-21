#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <stdbool.h>
#include <map/map.h>
#include <tools/animations/Animations.h>  // Tipos de animación (AnimState, Animation, AnimPlayer)

typedef struct Player {
    Vector2     pos;
    Vector2     size;
    float       velocityY;
    bool        isJumping;
    bool        isMovingLeft;

    // Sistema de animaciones
    AnimPlayer  ap;
} Player;

// Inicializa el jugador y sus animaciones
void InitPlayer(Player *p, Vector2 *playerSpawn);

// Actualiza lógica de jugador y animaciones, comprobando colisiones con plataformas
void UpdatePlayer(Player *p, float dt, Platform platforms[], int numPlatforms);

// Dibuja al jugador usando el sistema de animaciones
void DrawPlayer(const Player *p);

// Libera recursos del jugador (texturas de animaciones)
void UnloadPlayer(Player *p);

#endif // PLAYER_H
