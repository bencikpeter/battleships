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

    void renderTitle(GameEngine *engine);

protected:
    WonScene();

    // GameScene interface
public:
    void init(GameEngine *engine) override;
    void clean() override;
    void pause() override;
    void resume(GameEngine *engine) override;
    void runScene(GameEngine *engine) override;

    static WonScene* Instance();
};

#endif // WONSCENE_H
