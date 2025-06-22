#ifndef MENU_H
#define MENU_H

typedef enum
{
    SCENE_MENU,
    SCENE_GAME,
    SCENE_EXIT
} Scene;

extern int windowWidth;
extern int windowHeight;

void InitMenu(void);
void UpdateMenu(Scene *currentScene);
void DrawMenu(void);
void UnloadMenu(void);


#endif