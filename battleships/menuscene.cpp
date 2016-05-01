#include "menuscene.h"

MenuScene MenuScene::menuScene;

MenuScene::MenuScene()
{

}

void MenuScene::pause()
{

}

void MenuScene::resume()
{

}

void MenuScene::runScene(GameEngine *engine)
{
    if ( SDL_WaitEvent( &event ) ) {
        switch ( event.type ) {
        case value:
        case SDL_QUIT:
            engine->quit();
            break;
        case SDL_KEYDOWN:
            if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                engine->quit();
            }
            break;
        case SDL_MOUSEMOTION:
            //TODO check position of mouse
            break;
        default:
            break;
        }
    }
}

MenuScene *MenuScene::Instance()
{
    return &menuScene;
}
