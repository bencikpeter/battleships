#include "gamerenderer.h"
#include <iostream>

GameRenderer::GameRenderer():renderer( nullptr ) { }

void GameRenderer::init(SDL_Window *window) {
    this->window = window;
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if ( renderer == nullptr ) {
        std::cerr << "Error initializing renderer : " << SDL_GetError() << std::endl;
        exit(1);
    }

    int width;
    int height;

    SDL_GetWindowSize( window, &width, &height );
    SDL_RenderSetLogicalSize( renderer, width, height );
}

void GameRenderer::close()
{
    SDL_DestroyRenderer( renderer );
}

void GameRenderer::setRenderColor( Uint32 red, Uint32 green, Uint32 blue, Uint32 alfa ){
    SDL_SetRenderDrawColor( renderer, red, green, blue, alfa );
}

void GameRenderer::clear() {
    SDL_RenderClear( renderer );
}

void GameRenderer::render()
{
    SDL_RenderPresent( renderer );
}

void GameRenderer::renderCopy( SDL_Texture *texture, SDL_Rect &rect )
{
    SDL_RenderCopy( renderer, texture, nullptr, &rect );
}

void GameRenderer::computeRect(SDL_Texture *texture, int offset, SDL_Rect &rect)
{
    int width;
    int height;
    SDL_GetRendererOutputSize( renderer, &width, &height );
    SDL_QueryTexture( texture, nullptr, nullptr, &rect.w, &rect.h );
    rect.x = width/2 - rect.w/2;
    rect.y = offset;
}


