// draw.h
#ifndef DRAW_H
#define DRAW_H

#include <raylib.h>
#include <player/player.h>

// Inicializa parámetros de fondo, cámara, deadzone y look-ahead
void InitDrawingSystem(Texture2D bgTexture, float screenW, float screenH);

// Actualiza suavemente la cámara con zona muerta y look-ahead
void UpdateGameCamera(Camera2D *camera, float dt, const Player *player);

// Dibuja fondo, plataformas, jugador y pinchos en el orden correcto
void DrawGame(const Camera2D *camera, const Player *player);

#endif
