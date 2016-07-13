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


    void renderHostScene(GameEngine &engine);

protected:
    HostScene()
    {

    }

    // GameScene interface
public:
    void init(GameEngine &engine) override;
    void clean() override
    {
        MenuItem &waiting = menu[0];
        MenuItem &back = menu[1];

        SDL_DestroyTexture( waiting.notSelected );
        SDL_DestroyTexture( back.notSelected );
        SDL_DestroyTexture( back.selected );
    }
    void pause() override
    {

    }
    void resume(GameEngine &) override
    {

    }
    void runScene(GameEngine &engine) override;

    static HostScene& Instance()
    {
        return hostScene;
    }
};

#endif // HOSTSCENE_H
