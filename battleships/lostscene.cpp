#include "lostscene.h"

LostScene LostScene::lostScene;

void LostScene::renderTitle(GameEngine *engine)
{
    engine->renderer.clear();
    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, nullptr, &height );
    int offset = (height / 2) - (title->rect.h / 2);
    engine->renderer.computeRect( title->notSelected, offset, title->rect );
    engine->renderer.renderCopy( title->notSelected, title->rect );
    engine->renderer.render();
}

LostScene::LostScene()
{

}

void LostScene::init(GameEngine *engine)
{
    title = new MenuItem( "You lost", engine->font );
    SDL_Surface *lost = TTF_RenderText_Blended( engine->font, title->text.c_str(), { 255, 255, 255, 255 } );
    title->notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer, lost );
    SDL_FreeSurface( lost );
    title->computeDimmensions();
    renderTitle( engine );
}

void LostScene::clean()
{
    delete title;
}

void LostScene::pause()
{

}

void LostScene::resume(GameEngine *)
{

}

void LostScene::runScene(GameEngine *engine)
{
    if ( SDL_WaitEvent( &event ) ) {
        switch ( event.type ) {
        case SDL_QUIT:
            engine->quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            while ( engine->scenesNumber() != 1) {
                engine->popScene();
            }
            engine->reset();
            break;
        default:
            break;
        }
    }
}

LostScene *LostScene::Instance()
{
    return &lostScene;
}
