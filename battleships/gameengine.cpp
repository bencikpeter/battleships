#include "gameengine.h"
#include "gamescene.h"
#include "menuscene.h"

#include <iostream>

GameEngine::GameEngine( const char* title, int width, int height )
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) ) {
        std::cerr << "Failed to initialize SDL : " << SDL_GetError() << std::endl;
        exit(1);
    }

    if ( TTF_Init() == -1 ) {
        std::cerr << "Failed to initialize SDL_TTF : " << SDL_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont( "../battleships/a.ttf", 72 );

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
                               SDL_WINDOW_OPENGL );

    if ( window == nullptr ) {
        std::cerr << "There was error initializing the window : " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer.init( window );

    running = true;
}

GameEngine::~GameEngine() {
    TTF_CloseFont ( font );
    renderer.close();
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void GameEngine::changeScene(GameScene *scene) {
    if ( !scenes.empty() ) {
        scenes.top()->clean();
        scenes.pop();
    }
    scene->init( this );
    scenes.push( scene );
}

void GameEngine::pushScene(GameScene *scene) {
    if ( !scenes.empty() ) {
        scenes.top()->pause();
    }
    scene->init( this );
    scenes.push( scene );
}

void GameEngine::popScene() {
    if ( !scenes.empty() ) {
        scenes.top()->clean();
        scenes.pop();
    }

    if ( !scenes.empty() ) {
        scenes.top()->resume( this );
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

bool GameEngine::isRunning() const
{
    return running;
}
