#ifndef CONNECTSCENE_H
#define CONNECTSCENE_H

#include <string>
#include <vector>
#include "gamescene.h"
#include "menuitem.h"

class ConnectScene : public GameScene
{
private:
    static ConnectScene connectScene;
    SDL_Event event;
    std::vector<MenuItem> menu;

    void renderConnectScene(GameEngine *engine);

    void renderText( GameEngine *engine );


protected:
    ConnectScene();

    // GameScene interface
public:
    void pause() override;
    void resume(GameEngine *engine) override;
    void runScene(GameEngine *engine) override;
    void init(GameEngine *engine) override;
    void clean() override;

    static ConnectScene* Instance();
};

#endif // CONNECTSCENE_H
