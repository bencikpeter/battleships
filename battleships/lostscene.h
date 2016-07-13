#ifndef LOSTSCENE_H
#define LOSTSCENE_H

#include "menuitem.h"
#include "gamescene.h"

class LostScene : public GameScene
{
private:
    static LostScene lostScene;
    SDL_Event event;
    MenuItem* title;

    void renderTitle(GameEngine &engine);

protected:
    LostScene() {}

    // GameScene interface
public:
    void init(GameEngine &engine) override;
    void clean() override
    {
        delete title;
    }
    void pause() override {}
    void resume(GameEngine &) override
    {

    }
    void runScene(GameEngine &engine) override;

    static LostScene& Instance()
    {
        return lostScene;
    }
};

#endif // LOSTSCENE_H
