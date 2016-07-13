#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gameengine.h"

class GameScene
{

protected:
    GameScene() {}

    virtual ~GameScene() {}
public:
    virtual void init( GameEngine &engine ) = 0;

    virtual void clean() = 0;

    virtual void pause() = 0;

    virtual void resume( GameEngine &engine ) = 0;

    virtual void runScene( GameEngine &engine ) = 0;

    void changeState( GameEngine &engine, GameScene &scene )
    {
        engine.changeScene( scene );
    }

    GameScene( GameScene& other) = delete;

    GameScene& operator=( GameScene& other) = delete;
};

#endif // GAMESCENE_H
