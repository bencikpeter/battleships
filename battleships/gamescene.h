#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gameengine.h"

class GameScene
{

protected:
    GameScene();

    virtual ~GameScene();
public:
    virtual void pause() = 0;

    virtual void resume() = 0;

    virtual void runScene( GameEngine* engine ) = 0;

    void changeState( GameEngine* engine, GameScene* scene );

    GameScene( GameScene& other) = delete;

    GameScene& operator=( GameScene& other) = delete;
};

#endif // GAMESCENE_H
