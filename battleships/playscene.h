#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "gamescene.h"

class PlayScene : public GameScene
{
    using logic::Matrix;
private:
    static PlayScene playScene;
    SDL_Event event;
    std::future<std::pair> enemyShot;
    std::future<Matrix> enemyLayout;
    Matrix myGrid;
    Matrix enemyGrid;

    void renderMyGrid(GameEngine *engine);

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
