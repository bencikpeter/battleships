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
    std::pair< int, int > getMousePos()
    {
        int x;
        int y;
        std::pair< int, int > coordinates;
        SDL_GetMouseState( &x, &y );
        coordinates.first = x / cellWidth;
        coordinates.second = y / cellHeight;
        return coordinates;
    }
    void renderCell(GameEngine *engine, SDL_Rect *rect , int r, int g, int b)
    {
        engine->renderer.setRenderColor( r, g, b, 255 );
        SDL_RenderFillRect( engine->renderer.renderer, rect );
        engine->renderer.setRenderColor( 0, 0, 0, 255 );
        SDL_RenderDrawRect( engine->renderer.renderer, rect );
    }

protected:
    PlayScene()
    {

    }

    // GameScene interface
public:
    void init(GameEngine *engine) override;
    void clean() override
    {

    }
    void pause() override
    {

    }
    void resume(GameEngine *) override
    {

    }
    void runScene(GameEngine *engine) override;

    static PlayScene* Instance()
    {
        return &playScene;
    }
};

#endif // PLAYSCENE_H
