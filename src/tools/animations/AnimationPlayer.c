#include <stdlib.h>
#include <math.h>
#include <tools/animations/AnimationPlayer.h>
#include <player/player.h>
#define ASSETS_PATH "assets/"

// Inicializa todas las animaciones en el AnimPlayer
void InitAnimations(AnimPlayer *ap) {
    // IDLE
    Animation *idle = &ap->anims[ANIM_IDLE];
    idle->texture    = LoadTexture(ASSETS_PATH "idle.png");
    idle->frameCount = 4;
    idle->frameTime  = 0.15f;
    idle->loop       = true;
    idle->frames     = malloc(sizeof(Rectangle) * idle->frameCount);
    for (int i = 0; i < idle->frameCount; i++) {
        idle->frames[i] = (Rectangle){
            i * (idle->texture.width / idle->frameCount),
            0,
            idle->texture.width / idle->frameCount,
            idle->texture.height
        };
    }

    // WALK
    Animation *walk = &ap->anims[ANIM_WALK];
    walk->texture    = LoadTexture(ASSETS_PATH "walk.png");
    walk->frameCount = 4;
    walk->frameTime  = 0.1f;
    walk->loop       = true;
    walk->frames     = malloc(sizeof(Rectangle) * walk->frameCount);
    for (int i = 0; i < walk->frameCount; i++) {
        walk->frames[i] = (Rectangle){
            i * (walk->texture.width / walk->frameCount),
            0,
            walk->texture.width / walk->frameCount,
            walk->texture.height
        };
    }
/*
    // JUMP
    Animation *jump = &ap->anims[ANIM_JUMP];
    jump->texture    = LoadTexture(ASSETS_PATH "jump.png");
    jump->frameCount = 2;
    jump->frameTime  = 0.2f;
    jump->loop       = false;
    jump->frames     = malloc(sizeof(Rectangle) * jump->frameCount);
    for (int i = 0; i < jump->frameCount; i++) {
        jump->frames[i] = (Rectangle){
            i * (jump->texture.width / jump->frameCount),
            0,
            jump->texture.width / jump->frameCount,
            jump->texture.height
        };
    }

    // FALL
    Animation *fall = &ap->anims[ANIM_FALL];
    fall->texture    = LoadTexture(ASSETS_PATH "fall.png");
    fall->frameCount = 2;
    fall->frameTime  = 0.2f;
    fall->loop       = false;
    fall->frames     = malloc(sizeof(Rectangle) * fall->frameCount);
    for (int i = 0; i < fall->frameCount; i++) {
        fall->frames[i] = (Rectangle){
            i * (fall->texture.width / fall->frameCount),
            0,
            fall->texture.width / fall->frameCount,
            fall->texture.height
        };
    }
*/
    // Inicializar estado
    ap->state = ANIM_IDLE;
    ap->frame = 0;
    ap->timer = 0.0f;
}

// Libera texturas y memoria de los frames
void UnloadAnimations(AnimPlayer *ap) {
    for (int s = 0; s < ANIM_COUNT; s++) {
        UnloadTexture(ap->anims[s].texture);
        free(ap->anims[s].frames);
    }
}

// Actualiza la animación según el estado del jugador
void UpdateAnimation(AnimPlayer *ap, float dt, void *playerCtx) {
    // Cast al tipo correcto
    Player *p = (Player *)playerCtx;

    // Determinar nuevo estado
    AnimState newState = ap->state;
    if (p->velocityY < 0)         newState = ANIM_JUMP;
    else if (p->velocityY > 0)    newState = ANIM_FALL;
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) newState = ANIM_WALK;
    else                          newState = ANIM_IDLE;

    // Transición de estado
    if (newState != ap->state) {
        ap->state = newState;
        ap->frame = 0;
        ap->timer = 0.0f;
    }

    // Avanza el frame
    Animation *A = &ap->anims[ap->state];
    ap->timer += dt;
    if (ap->timer >= A->frameTime) {
        ap->timer -= A->frameTime;
        ap->frame++;
        if (ap->frame >= A->frameCount) {
            if (A->loop) ap->frame = 0;
            else         ap->frame = A->frameCount - 1;
        }
    }
}

// Dibuja la animación actual en pantalla
void DrawAnimation(const AnimPlayer *ap, Vector2 pos, bool flipX) {
    const Animation *A = &ap->anims[ap->state];
    Rectangle src = A->frames[ap->frame];
    if (flipX) src.width = -src.width;
    Rectangle dst = { pos.x, pos.y, fabsf(src.width), src.height };
    DrawTexturePro(A->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}
