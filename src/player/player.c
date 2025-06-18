#include <player/player.h>
#include <stdlib.h>
#include <math.h>

#define GRAVITY      800
#define JUMP_FORCE  -300
#define MOVE_SPEED   200

// --- Helpers de animación ---

static void InitAnimations(AnimPlayer *ap) {
    // Idle
    ap->anims[ANIM_IDLE].texture   = LoadTexture("assets/idle.png");
    ap->anims[ANIM_IDLE].frameCount= 4;
    ap->anims[ANIM_IDLE].frameTime = 0.15f;
    ap->anims[ANIM_IDLE].loop      = true;
    ap->anims[ANIM_IDLE].frames    = malloc(sizeof(Rectangle)*4);
    for (int i = 0; i < 4; i++) {
        ap->anims[ANIM_IDLE].frames[i] = (Rectangle){
            i * (ap->anims[ANIM_IDLE].texture.width/4),
            0,
            ap->anims[ANIM_IDLE].texture.width/4,
            ap->anims[ANIM_IDLE].texture.height
        };
    }
    // Walk
    ap->anims[ANIM_WALK].texture   = LoadTexture("assets/walk.png");
    ap->anims[ANIM_WALK].frameCount= 4;
    ap->anims[ANIM_WALK].frameTime = 0.1f;
    ap->anims[ANIM_WALK].loop      = true;
    ap->anims[ANIM_WALK].frames    = malloc(sizeof(Rectangle)*4);
    for (int i = 0; i < 4; i++) {
        ap->anims[ANIM_WALK].frames[i] = (Rectangle){
            i * (ap->anims[ANIM_WALK].texture.width/4),
            0,
            ap->anims[ANIM_WALK].texture.width/4,
            ap->anims[ANIM_WALK].texture.height
        };
    }
    // Jump & Fall (puedes usar el mismo spritesheet, distinto frameCount si quieres)
    ap->anims[ANIM_JUMP] = ap->anims[ANIM_IDLE];
    ap->anims[ANIM_FALL] = ap->anims[ANIM_IDLE];

    // Estado inicial
    ap->state  = ANIM_IDLE;
    ap->frame  = 0;
    ap->timer  = 0;
}

static void UnloadAnimations(AnimPlayer *ap) {
    for (int s = 0; s < ANIM_COUNT; s++) {
        UnloadTexture(ap->anims[s].texture);
        free(ap->anims[s].frames);
    }
}

static void UpdateAnimation(AnimPlayer *ap, float dt, Player *p) {
    // 1) Determinar nuevo estado según la lógica del jugador
    AnimState newState = ap->state;
    if (p->velocityY < 0)        newState = ANIM_JUMP;
    else if (p->velocityY > 0)   newState = ANIM_FALL;
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
                                 newState = ANIM_WALK;
    else                          newState = ANIM_IDLE;

    // 2) Si cambia de estado, reiniciar anim
    if (newState != ap->state) {
        ap->state = newState;
        ap->frame = 0;
        ap->timer = 0;
    }

    // 3) Avanzar frame
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

static void DrawAnimation(const AnimPlayer *ap, Vector2 pos, bool flipX) {
    const Animation *A = &ap->anims[ap->state];
    Rectangle src = A->frames[ap->frame];
    if (flipX) src.width = -src.width;
    Rectangle dst = { pos.x, pos.y, fabs(src.width), src.height };
    DrawTexturePro(A->texture, src, dst, (Vector2){0,0}, 0, WHITE);
}

// --- Funciones de Player ---

#include <player/player.h>
#include <stdlib.h>

#define GRAVITY      800.0f
#define JUMP_FORCE  -300.0f
#define MOVE_SPEED   200.0f

void InitPlayer(Player *p) {
    // Posición y tamaño iniciales
    p->pos = (Vector2){ 200, 300 };
    p->size = (Vector2){ 14, 19 }; // x, y
    p->velocityY = 0;
    p->isJumping = false;
    p->isMovingLeft = false;
    // Inicializa tu sistema de animaciones
    InitAnimations(&p->ap);
}

void UpdatePlayer(Player *p, float dt, Platform platforms[], int numPlatforms) {
    // 1) Movimiento horizontal
    float moveX = 0;
    if (IsKeyDown(KEY_LEFT)) {
        moveX = -MOVE_SPEED * dt;
        p->isMovingLeft = true;
    } else if (IsKeyDown(KEY_RIGHT)) {
        moveX =  MOVE_SPEED * dt;
        p->isMovingLeft = false;
    }
    p->pos.x += moveX;

    // Comprueba colisión en X con cada plataforma
    Rectangle playerRect = { p->pos.x, p->pos.y, p->size.x, p->size.y };
    for (int i = 0; i < numPlatforms; i++) {
        if (!platforms[i].active) continue;
        if (CheckCollisionRecs(playerRect, platforms[i].rec)) {
            // Resolver colisión horizontal
            if (moveX > 0) {
                // viniendo desde la izquierda
                p->pos.x = platforms[i].rec.x - p->size.x;
            } else if (moveX < 0) {
                // viniendo desde la derecha
                p->pos.x = platforms[i].rec.x + platforms[i].rec.width;
            }
            playerRect.x = p->pos.x;
        }
    }

    // 2) Saltar
    if (IsKeyPressed(KEY_C) && !p->isJumping) {
        p->velocityY = JUMP_FORCE;
        p->isJumping = true;
    }

    // 3) Aplicar gravedad
    p->velocityY += GRAVITY * dt;
    p->pos.y += p->velocityY * dt;

    // Comprueba colisión en Y con cada plataforma
    playerRect.y = p->pos.y;
    for (int i = 0; i < numPlatforms; i++) {
        if (!platforms[i].active) continue;
        Rectangle plat = platforms[i].rec;
        if (CheckCollisionRecs(playerRect, plat)) {
            if (p->velocityY > 0) {
                // estaba cayendo: apoyar sobre la plataforma
                p->pos.y = plat.y - p->size.y;
                p->isJumping = false;
            } else if (p->velocityY < 0) {
                // subiendo: chocar la cabeza
                p->pos.y = plat.y + plat.height;
            }
            p->velocityY = 0;
            playerRect.y = p->pos.y;
        }
    }

    // 4) Actualizar animación
    UpdateAnimation(&p->ap, dt, p);
}

void DrawPlayer(const Player *p) {
    // Dibuja la animación actual en p->pos
    DrawAnimation(&p->ap, p->pos, p->isMovingLeft);
}

void UnloadPlayer(Player *p) {
    UnloadAnimations(&p->ap);
}
