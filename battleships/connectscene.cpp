#include "connectscene.h"
#include <iostream>
#include "playscene.h"

ConnectScene ConnectScene::connectScene;

void ConnectScene::renderConnectScene(GameEngine *engine)
{
    engine->renderer.clear();
    renderText( engine );
    int width;
    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, &width, &height );
    int offset = height / 4;
    int i = 0;
    for ( MenuItem &item : menu ) {
        engine->renderer.computeRect( item.notSelected, i, item.rect );
        i += offset;
        engine->renderer.renderCopy( item.notSelected, item.rect );
    }

    for ( int i = 2; i < 4; ++i ) {
        if ( menu[i].isSelected() ) {
            engine->renderer.renderCopy( menu[i].selected, menu[i].rect );
        } else {
            engine->renderer.renderCopy( menu[i].notSelected, menu[i].rect );
        }
    }
    engine->renderer.render();
}

void ConnectScene::renderText(GameEngine *engine)
{
    MenuItem &text = menu[1];

    if ( text.notSelected != nullptr ) {
        SDL_DestroyTexture( text.notSelected );
        text.notSelected = nullptr;
    }
    if ( text.selected != nullptr ) {
        SDL_DestroyTexture( text.selected );
        text.selected = nullptr;
    }

    SDL_Surface *notSelected = TTF_RenderText_Blended( engine->font,
                                                       text.text.c_str(),
                                                       { 255, 255, 255, 255 } );

    text.notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                     notSelected );

    SDL_FreeSurface( notSelected );
    text.computeDimmensions();
}

ConnectScene::ConnectScene()
{

}

void ConnectScene::pause()
{
    SDL_StopTextInput();
}

void ConnectScene::resume(GameEngine *)
{
    SDL_StartTextInput();
}

void ConnectScene::runScene(GameEngine *engine)
{
    std::string &inputText = menu[1].text;

    if ( SDL_WaitEvent( &event ) ) {
        switch ( event.type ) {
        case SDL_QUIT:
            engine->quit();
            break;
        case SDL_MOUSEMOTION:
            renderConnectScene( engine );
            break;
        case SDL_MOUSEBUTTONDOWN:
            if ( menu[2].isSelected() ) {
                engine->popScene();
            }
            if ( menu[3].isSelected() ) {
                if ( engine->logic.isIpValid( inputText ) ) {
                    connectionSuccesful = std::async( std::launch::async,
                                                      &logic::Logic::connect,
                                                      &engine->logic,
                                                      inputText );
                }
            }
            break;
        case SDL_KEYDOWN:
            if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ) {
                inputText.pop_back();
                renderConnectScene( engine );
            }
            break;
        case SDL_TEXTINPUT:
            inputText += event.text.text;
            renderConnectScene( engine );
            break;
        }
        if ( engine->logicEventType == event.type && event.user.code == CONNECT ) {
            if ( connectionSuccesful.get() ){
                // na niekoho som sa pripojil
                LogicEvent ev( CONNECT ); // event for PlayScene to recognize, that this client is connected to host
                engine->pushScene( PlayScene::Instance() );
            }
        }
    }
}

ConnectScene *ConnectScene::Instance()
{
    return &connectScene;
}

void ConnectScene::init(GameEngine *engine)
{
    SDL_StartTextInput();

    menu.push_back( MenuItem( "Enter IP address", engine->font ) );
    menu.push_back( MenuItem( "", engine->font ) );
    menu.push_back( MenuItem( "Back", engine->font ) );
    menu.push_back( MenuItem( "Join", engine->font ) );

    for ( MenuItem &item : menu ) {
        SDL_Surface *notSelected = TTF_RenderText_Blended( engine->font,
                                                           item.text.c_str(),
                                                           { 255, 255, 255, 255 } );

        item.notSelected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                         notSelected );

        SDL_FreeSurface( notSelected );
        item.computeDimmensions();
    }

    for ( int i = 2; i < 4; ++i ) {
        SDL_Surface *selected = TTF_RenderText_Blended( engine->font,
                                                        menu[i].text.c_str(),
                                                        { 0, 255, 0, 255 } );

        menu[i].selected = SDL_CreateTextureFromSurface( engine->renderer.renderer,
                                                         selected );
        SDL_FreeSurface( selected );
    }

    renderConnectScene( engine );
}

void ConnectScene::clean()
{
    SDL_StopTextInput();
}
