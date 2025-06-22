// scene/scene.h
#ifndef SCENE_H
#define SCENE_H

typedef struct Scene {
	void (*Start)(void);
	void (*Update)(float dt);
	void (*Draw)(void);
	void (*Unload)(void);
} Scene;

extern Scene *currentScene;

void SetScene(Scene *scene);
void UpdateScene(float dt);
void DrawScene(void);
void UnloadCurrentScene(void);

#endif
