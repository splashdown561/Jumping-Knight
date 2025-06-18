#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H

#include <stdbool.h>      // para bool
#include <tools/animations/Animations.h>

void InitAnimations(AnimPlayer *ap);
void UnloadAnimations(AnimPlayer *ap);
void UpdateAnimation(AnimPlayer *ap, float dt, /* puntero a tu Player si lo necesitas */ void *playerCtx);
void DrawAnimation(const AnimPlayer *ap, Vector2 pos, bool flipX);

#endif // ANIMATIONPLAYER_H
