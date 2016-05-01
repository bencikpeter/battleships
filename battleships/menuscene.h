#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "gamescene.h"

class MenuScene : public GameScene
{
private:
    static MenuScene menuScene;
    SDL_Event event;

protected:
    MenuScene();

public:
    // GameScene interface
    void pause() override;
    void resume() override;
    void runScene(GameEngine *engine) override;

    static MenuScene* Instance();

};

#endif // MENUSCENE_H
