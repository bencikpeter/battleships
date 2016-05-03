#ifndef HOSTSCENE_H
#define HOSTSCENE_H

#include "gamescene.h"
#include "menuitem.h"
#include <vector>

class HostScene : public GameScene
{
private:
    static HostScene hostScene;
    SDL_Event event;
    std::vector<MenuItem> menu;
    std::future<bool> host;


    void renderHostScene( GameEngine *engine, SDL_Event &event );

protected:
    HostScene();

    // GameScene interface
public:
    void init(GameEngine *engine) override;
    void clean() override;
    void pause() override;
    void resume(GameEngine *engine) override;
    void runScene(GameEngine *engine) override;

    static HostScene* Instance();
};

#endif // HOSTSCENE_H
