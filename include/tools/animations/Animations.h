#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <raylib.h>

// Estados de animación
typedef enum {
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_JUMP,
    ANIM_FALL,
    ANIM_COUNT
} AnimState;

// Datos de una animación
typedef struct {
    Texture2D   texture;
    Rectangle*  frames;
    int         frameCount;
    float       frameTime;
    bool        loop;
} Animation;

// “Reproductor” de animaciones
typedef struct {
    Animation   anims[ANIM_COUNT];
    AnimState   state;
    int         frame;
    float       timer;
} AnimPlayer;

#endif // ANIMATIONS_H
