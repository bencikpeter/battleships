#ifndef WONSCENE_H
#define WONSCENE_H

#include "menuitem.h"
#include "gamescene.h"

class WonScene : public GameScene
{
private:
    static WonScene wonScene;
    SDL_Event event;
    MenuItem *title;

    void renderTitle(GameEngine &engine);

protected:
    WonScene()
    {

    }

    // GameScene interface
public:
    void init(GameEngine &engine) override;
    void clean() override
    {
        delete title;
    }
    void pause() override
    {

    }
    void resume(GameEngine &) override
    {

    }
    void runScene(GameEngine &engine) override;

    static WonScene& Instance()
    {
        return wonScene;
    }
};

#endif // WONSCENE_H
