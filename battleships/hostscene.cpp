#include "hostscene.h"

HostScene HostScene::hostScene;

void HostScene::renderHostScene(GameEngine *engine, SDL_Event &event)
{
    engine->renderer.clear();

    MenuItem &waiting = menu[0];
    MenuItem &back = menu[1];

    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, nullptr, &height );
    int i = height / 2 - menu[0].rect.h;

    for ( MenuItem &item : menu ) {
        engine->renderer.computeRect( item.notSelected, i, item.rect );
        i += item.rect.h;
    }
    engine->renderer.renderCopy( waiting.notSelected, waiting.rect );

    if ( back.isSelected() ) {
        engine->renderer.renderCopy( back.selected, back.rect );
    } else {
        engine->renderer.renderCopy( back.notSelected, back.rect );
    }

    engine->renderer.render();
}

HostScene::HostScene()
{

}

void HostScene::init(GameEngine *engine)
{
    menu.push_back( MenuItem( "Waiting for other player", engine->font ) );
    menu.push_back( MenuItem( "Back", engine->font ) );

    MenuItem &waiting = menu[0];
    MenuItem &back = menu[1];

    SDL_Surface *waitingNotSelected = TTF_RenderText_Blended( engine->font,
                                                              waiting.text.c_str(),
                                                              { 255, 255, 255, 255 } );

    SDL_Surface *backNotSelected = TTF_RenderText_Blended( engine->font,
                                                           back.text.c_str(),
                                                           { 255, 255, 255, 255 } );

    SDL_Surface *backSelected = TTF_RenderText_Blended( engine->font,
                                                        back.text.c_str(),
                                                        { 0, 255, 0, 255 } );

    waiting.notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                        waitingNotSelected );

    back.notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                     backNotSelected );

    back.selected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                  backSelected );

    SDL_FreeSurface ( waitingNotSelected );
    SDL_FreeSurface ( backNotSelected );
    SDL_FreeSurface ( backSelected );

    waiting.computeDimmensions();
    back.computeDimmensions();

    renderHostScene( engine, event );
}

void HostScene::clean()
{
    MenuItem &waiting = menu[0];
    MenuItem &back = menu[1];

    SDL_DestroyTexture( waiting.notSelected );
    SDL_DestroyTexture( back.notSelected );
    SDL_DestroyTexture( back.selected );
}

void HostScene::pause()
{

}

void HostScene::resume(GameEngine *engine)
{

}

void HostScene::runScene(GameEngine *engine)
{
    if( SDL_WaitEvent( &event ) ) {
        switch ( event.type ){
        case SDL_QUIT:
            engine->quit();
            break;
        case SDL_MOUSEMOTION:
            renderHostScene( engine, event );
            break;
        case SDL_MOUSEBUTTONDOWN:
            if ( menu[1].isSelected() ) {
                engine->popScene();
            }
            break;
        }
    }
}

HostScene *HostScene::Instance()
{
    return &hostScene;
}
