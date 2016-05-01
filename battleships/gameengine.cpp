#include "gameengine.h"

#include <iostream>

GameEngine::GameEngine( const char* title, int width, int height )
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) ) {
        std::cerr << "Failed to initialize SDL : " << SDL_GetError() << endl;
        exit(1);
    }

    logicEventType = SDL_RegisterEvents( 1 );

    if ( logicEventType == ( Uint32 ) - 1 ) {
        std::cerr << "Failed to initialize business logic event : " << SDL_GetError() << std::endl;
        exit(1);
    }

    LogicEvent::setType( logicEventType );

    window = SDL_CreateWindow( title,
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width,
                               height,
                               SDL_WINDOW_RESIZABLE );

    if ( window == nullptr ) {
        std::cerr << "There was error initializing the window : " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer.init( window );
}

GameEngine::~GameEngine() {
    renderer.close();
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void GameEngine::changeScene(GameScene *scene) {
    if ( !scenes.empty() ) {
        scenes.pop();
    }

    scenes.push( scene );
}

void GameEngine::pushScene(GameScene *scene) {
    if ( !scenes.empty() ) {
        scenes.top()->pause();
    }

    scenes.push( scene );
}

void GameEngine::popScene() {
    if ( !scenes.empty() ) {
        scenes.pop();
    }

    if ( !scenes.empty() ) {
        scenes.top()->resume();
    }
}

void GameEngine::runScene() {
    scenes.top()->runScene( this );
}

void GameEngine::quit()
{
    running = false;
}

Uint32 GameEngine::getLogicEventType() const
{
    return logicEventType;
}
