// scene/scene.c
#include <tools/SceneManager/Scene.h>
#include <stdlib.h>

Scene *currentScene = NULL;

void SetScene(Scene *scene) {
	if (currentScene && currentScene->Unload) currentScene->Unload();
	currentScene = scene;
	if (currentScene && currentScene->Start) currentScene->Start();
}

void UpdateScene(float dt) {
	if (currentScene && currentScene->Update) currentScene->Update(dt);
}

void DrawScene(void) {
	if (currentScene && currentScene->Draw) currentScene->Draw();
}

void UnloadCurrentScene(void) {
	if (currentScene && currentScene->Unload) currentScene->Unload();
	currentScene = NULL;
}
