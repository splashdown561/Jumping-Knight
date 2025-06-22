#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);

bool ShouldResetGame(void);
void ResetGame(void);
#endif