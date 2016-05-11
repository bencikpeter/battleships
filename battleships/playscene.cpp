#include "playscene.h"

PlayScene PlayScene::playScene;

void PlayScene::renderMyGrid(GameEngine *engine)
{
    int width;
    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, &width, &height );
    int cellWidth = width / 10;
    int cellHeight = height / 10;
    for (int i = 0; i < 10; ++i ){
        for ( int j = 0; j < 10; ++j ) {
            SDL_Rect rect;
            rect.w = cellWidth;
            rect.h = cellHeight;
            rect.x = i * cellWidth;
            rect.h = j * cellHeight;
            if ( myGrid.get()[ i ][ j ] == logic::WATER_NOT_SHOT ) {
                engine->renderer.setRenderColor( 64, 164, 223, 255 );
            } else if ( myGrid.get()[ i ][ j ] == logic::WATER_SHOT ) {
                engine->renderer.setRenderColor( 0, 0, 139, 255 );
            } else if ( myGrid.get()[ i ][ j ] == logic::SHIP_NOT_SHOT ) {
                engine->renderer.setRenderColor( 130, 82, 1, 255 );
            } else if ( myGrid.get()[ i ][ j ] == logic::SHIP_SHOT ) {
                engine->renderer.setRenderColor( 0, 0, 0, 255 );
            } else if ( myGrid.get()[ i ][ j ] == logic::CLICKABLE ) {
                engine->renderer.setRenderColor( 64, 164, 223, 255 );
            } else if ( myGrid.get()[ i ][ j ] == logic::NOT_CLICKABLE ) {
                engine->renderer.setRenderColor( 116, 127, 151, 255 );
            }
            SDL_RenderFillRect( engine->renderer.renderer, &rect);
        }
    }
    engine->renderer.render();
}

PlayScene::PlayScene()
{

}

void PlayScene::init(GameEngine *engine)
{
    for ( int i = 0; i < 10; ++i ) {
        for ( int j = 0; j < 10; ++j ) {
            if ( j & 1 == 1 && i & 1 == 0) {
                myGrid.get()[ i ][ j ] = logic::WATER_NOT_SHOT;
            } else {
                myGrid.get()[ i ][ j ] = logic::SHIP_NOT_SHOT;
            }
        }
    }
}

void PlayScene::clean()
{

}

void PlayScene::pause()
{

}

void PlayScene::resume(GameEngine *engine)
{
    if ( SDL_WaitEvent( &event ) ) {
        switch ( event.type ) {
        case SDL_QUIT:
            engine->quit();
            break;
        }
    }
}

void PlayScene::runScene(GameEngine *engine)
{

}

PlayScene *PlayScene::Instance()
{
    return &playScene;
}
