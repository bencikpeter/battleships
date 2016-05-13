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
    std::future<void> temp;
    logic::Matrix myGrid;
    logic::Matrix enemyGrid;
    int phase;
    int cellWidth;
    int cellHeight;
    bool clicked;
    std::pair< int, int > shipBegin;
    std::pair< int, int > shipEnd;
    std::pair< int, int > mouseCoord;
    std::pair< int, int > shot;
    bool shipsPlaced;
    bool ready;
    bool host;
    bool layoutReceived;
    bool layoutSent;
    bool getLayout;
    bool getShot;

    void renderMyGrid(GameEngine *engine);
    void renderEnemyGrid(GameEngine *engine);
    std::pair< int, int > getMousePos();
    void renderCell(GameEngine *engine, SDL_Rect *rect , int r, int g, int b);

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
