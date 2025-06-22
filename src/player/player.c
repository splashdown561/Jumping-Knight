#include <player/player.h>
#include <stdlib.h>
#include <math.h>
#include <obj/spike.h>
#include <CSV/csvreader.h>
#include <stdio.h>

#define GRAVITY     800.0f
#define JUMP_FORCE -300.0f
#define MOVE_SPEED  200.0f

bool grounded = false;

static Animation CreateAnim(const char *path, int frames, float time) {
	Animation a;
	a.texture = LoadTexture(path);
	a.frameCount = frames;
	a.frameTime = time;
	a.loop = true;
	a.frames = malloc(sizeof(Rectangle) * frames);
	int w = a.texture.width / frames, h = a.texture.height;
	for (int i = 0; i < frames; i++)
		a.frames[i] = (Rectangle){ i * w, 0, w, h };
	return a;
}

static void InitAnimations(AnimPlayer *ap) {
	ap->anims[ANIM_IDLE] = CreateAnim("assets/idle.png", 4, 0.15f);
	ap->anims[ANIM_WALK] = CreateAnim("assets/walk.png", 4, 0.10f);
	ap->anims[ANIM_JUMP] = ap->anims[ANIM_IDLE];
	ap->anims[ANIM_FALL] = ap->anims[ANIM_IDLE];
	ap->state = ANIM_IDLE;
	ap->frame = ap->timer = 0;
}

static void UnloadAnimations(AnimPlayer *ap) {
	for (int i = 0; i < ANIM_COUNT; i++) {
		UnloadTexture(ap->anims[i].texture);
		free(ap->anims[i].frames);
	}
}

static void UpdateAnimation(AnimPlayer *ap, float dt, Player *p) {
	AnimState newState =
		p->velocityY < 0 ? ANIM_JUMP :
		p->velocityY > 0 ? ANIM_FALL :
		(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) ? ANIM_WALK :
		ANIM_IDLE;

	if (newState != ap->state) ap->state = newState, ap->frame = ap->timer = 0;
	Animation *a = &ap->anims[ap->state];
	if ((ap->timer += dt) >= a->frameTime) {
		ap->timer -= a->frameTime;
		ap->frame = (ap->frame + 1) % a->frameCount;
	}
}

static void DrawAnimation(const AnimPlayer *ap, Vector2 pos, bool flipX) {
	const Animation *a = &ap->anims[ap->state];
	Rectangle src = a->frames[ap->frame];
	if (flipX) src.width = -src.width;
	Rectangle dst = { pos.x, pos.y, fabs(src.width), src.height };
	DrawTexturePro(a->texture, src, dst, (Vector2){0}, 0, WHITE);
}

void InitPlayer(Player *p, Vector2 *spawn) {
	p->pos = *spawn;
	p->size = (Vector2){14, 19};
	p->velocityY = 0;
	p->isJumping = false;
	p->isMovingLeft = false;
	InitAnimations(&p->ap);
}

void UpdatePlayer(Player *p, float dt, Platform platforms[], int numPlatforms) {
	float moveX = 0;
	if (IsKeyDown(KEY_LEFT)) moveX = -MOVE_SPEED * dt, p->isMovingLeft = true;
	if (IsKeyDown(KEY_RIGHT)) moveX =  MOVE_SPEED * dt, p->isMovingLeft = false;
	if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) && IsKeyDown(KEY_LEFT_SHIFT))
		moveX *= 1.5;
	p->pos.x += moveX;

	Rectangle r = { p->pos.x, p->pos.y, p->size.x, p->size.y };
	for (int i = 0; i < numPlatforms; i++) {
		if (!platforms[i].active || !CheckCollisionRecs(r, platforms[i].rec)) continue;
		p->pos.x = moveX > 0 ? platforms[i].rec.x - p->size.x :
				moveX < 0 ? platforms[i].rec.x + platforms[i].rec.width : p->pos.x;
		r.x = p->pos.x;
	}

	Rectangle feet = { p->pos.x, p->pos.y + p->size.y + 1, p->size.x, 2 };
	for (int i = 0; i < numPlatforms && !grounded; i++)
		if (platforms[i].active && CheckCollisionRecs(feet, platforms[i].rec)) grounded = true;

	if (IsKeyPressed(KEY_C) && grounded)
		p->velocityY = JUMP_FORCE;

	p->velocityY += GRAVITY * dt;
	p->pos.y += p->velocityY * dt;
	r.y = p->pos.y;

	for (int i = 0; i < numPlatforms; i++) {
		if (!platforms[i].active || !CheckCollisionRecs(r, platforms[i].rec)) continue;
		if (p->velocityY > 0) p->pos.y = platforms[i].rec.y - p->size.y;
		else				  p->pos.y = platforms[i].rec.y + platforms[i].rec.height;
		p->velocityY = 0;
		r.y = p->pos.y;
	}

	UpdateAnimation(&p->ap, dt, p);

	for (int i = 0; i < numSpikes; i++) {
		if (CheckCollisionRecs(r, spikes[i].rec)) {
			p->pos = playerSpawn;
			p->velocityY = 0;
			printf("[Spike] Colisión. Reinicio a (%.1f, %.1f)\n", playerSpawn.x, playerSpawn.y);
			break;
		}
	}
}

void DrawPlayer(const Player *p) {
	DrawAnimation(&p->ap, p->pos, p->isMovingLeft);
}

void UnloadPlayer(Player *p) {
	UnloadAnimations(&p->ap);
}
