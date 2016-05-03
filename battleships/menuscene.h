#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <vector>
#include <menuitem.h>
#include <gamescene.h>

class MenuScene : public GameScene
{
private:
    static MenuScene menuScene;
    SDL_Event event;
    std::vector<MenuItem> menu;

    void renderMenu(GameEngine *engine, SDL_Event &event );

friend GameEngine;

protected:
    MenuScene();

public:
    // GameScene interface
    void pause() override;
    void resume(GameEngine *engine) override;
    void runScene(GameEngine *engine) override;
    void clean() override;
    void init(GameEngine *engine) override;

    static MenuScene* Instance();
};

#endif // MENUSCENE_H
