#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "gamescene.h"

class PlayScene : public GameScene
{
private:
    static PlayScene playScene;
    SDL_Event event;
    std::future<std::pair< int,int > > enemyShot;
    std::future<logic::Matrix> enemyLayout;
    logic::Matrix myGrid;
    logic::Matrix enemyGrid;
    int phase;
    int cellWidth;
    int cellHeight;

    void renderMyGrid(GameEngine *engine);
    std::pair< int, int > getMousePos();

protected:
    PlayScene();

    // GameScene interface
public:
    void init(GameEngine *engine) override;
    void clean() override;
    void pause() override;
    void resume(GameEngine *engine) override;
    void runScene(GameEngine *engine) override;

    static PlayScene* Instance();
};

#endif // PLAYSCENE_H
