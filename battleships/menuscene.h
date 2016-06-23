#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <vector>
#include "menuitem.h"
#include "gamescene.h"

class MenuScene : public GameScene
{
private:
    static MenuScene menuScene;
    SDL_Event event;
    std::vector<MenuItem> menu;

    void renderMenu(GameEngine *engine);

friend GameEngine;

protected:
MenuScene()
{

}

public:
    // GameScene interface
void pause() override
{

}
void resume(GameEngine *engine) override
{
    renderMenu( engine );
}
    void runScene(GameEngine *engine) override;
    void clean() override
    {
        for ( MenuItem &item : menu ) {
            SDL_DestroyTexture( item.selected );
            SDL_DestroyTexture( item.notSelected );
        }
    }
    void init(GameEngine *engine) override;

    static MenuScene* Instance()
    {
        return &menuScene;
    }
};

#endif // MENUSCENE_H
