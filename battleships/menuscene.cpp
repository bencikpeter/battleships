#include "menuscene.h"
#include "iostream"
#include "connectscene.h"
#include "hostscene.h"

MenuScene MenuScene::menuScene;

void MenuScene::renderMenu(GameEngine *engine, SDL_Event &event) {
    engine->renderer.clear();

    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, nullptr, &height );
    int i = height / 2 - menu[0].rect.h;
    for ( MenuItem &item : menu ) {
        engine->renderer.computeRect( item.notSelected, i, item.rect );
        if ( item.isSelected() ) {
            engine->renderer.renderCopy( item.selected, item.rect );
        } else {
            engine->renderer.renderCopy( item.notSelected, item.rect );
        }
        i += item.rect.h;
    }
    engine->renderer.render();
}

MenuScene::MenuScene()
{

}

void MenuScene::pause()
{

}

void MenuScene::resume(GameEngine *engine)
{
    renderMenu( engine, event );
}

void MenuScene::runScene(GameEngine *engine)
{
    if ( SDL_WaitEvent( &event ) ) {
        switch ( event.type ) {
        case SDL_QUIT:
            engine->quit();
            break;
        case SDL_MOUSEMOTION:
            renderMenu( engine, event );
            break;
        case SDL_MOUSEBUTTONDOWN:
            if ( menu[0].isSelected() ) {
                engine->pushScene( HostScene::Instance() );
            }
            if ( menu[1].isSelected() ) {
                engine->pushScene( ConnectScene::Instance() );
            }
            break;
        case SDL_WINDOWEVENT:
            renderMenu( engine, event );
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

void MenuScene::init(GameEngine *engine)
{
    menu.push_back( MenuItem( "Host", engine->font ) );
    menu.push_back( MenuItem( "Connect", engine->font ) );

    for ( MenuItem &item : menu ) {
        SDL_Surface *notSelected = TTF_RenderText_Blended( engine->font, item.text.c_str(), { 255, 255, 255, 255 } );
        SDL_Surface *selected = TTF_RenderText_Blended( engine->font, item.text.c_str(), { 0, 255, 0, 255 } );
        item.notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer, notSelected );
        item.selected = SDL_CreateTextureFromSurface( engine->renderer.renderer, selected );
        SDL_FreeSurface( notSelected );
        SDL_FreeSurface( selected );
        item.computeDimmensions();
    }
    renderMenu( engine, event );
}

void MenuScene::clean()
{
    for ( MenuItem &item : menu ) {
        SDL_DestroyTexture( item.selected );
        SDL_DestroyTexture( item.notSelected );
    }
}
