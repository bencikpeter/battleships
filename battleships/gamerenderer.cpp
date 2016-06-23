#include "gamerenderer.h"
#include <iostream>

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
